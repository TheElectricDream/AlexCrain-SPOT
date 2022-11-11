//
//  Parker_Velocity_Control.cpp
//
//  Academic License - for use in teaching, academic research, and meeting
//  course requirements at degree granting institutions only.  Not for
//  government, commercial, or other organizational use.
//
//  Code generation for model "Parker_Velocity_Control".
//
//  Model version              : 1.645
//  Simulink Coder version : 9.3 (R2020a) 18-Nov-2019
//  C++ source code generated on : Wed Jul 13 11:50:24 2022
//
//  Target selection: ert.tlc
//  Embedded hardware selection: ARM Compatible->ARM Cortex
//  Code generation objective: Execution efficiency
//  Validation result: Not run


#include "Parker_Velocity_Control.h"
#include "Parker_Velocity_Control_private.h"

// Block signals (default storage)
B_Parker_Velocity_Control_T Parker_Velocity_Control_B;

// Block states (default storage)
DW_Parker_Velocity_Control_T Parker_Velocity_Control_DW;

// Real-time model
RT_MODEL_Parker_Velocity_Cont_T Parker_Velocity_Control_M_ =
  RT_MODEL_Parker_Velocity_Cont_T();
RT_MODEL_Parker_Velocity_Cont_T *const Parker_Velocity_Control_M =
  &Parker_Velocity_Control_M_;

// Forward declaration for local functions
static real_T Park_rt_urand_Upu32_Yd_f_pw_snf(uint32_T *u);
static real_T Park_rt_nrand_Upu32_Yd_f_pw_snf(uint32_T *u);
static real_T Parker_Velocity_Control_xnrm2(int32_T n, const real_T x[36],
  int32_T ix0);
static real_T Parker_Velocity_Control_xnrm2_l(int32_T n, const real_T x[6],
  int32_T ix0);
static void Parker_Velocity_Contro_xaxpy_ot(int32_T n, real_T a, const real_T x
  [6], int32_T ix0, real_T y[36], int32_T iy0);
static void Parker_Velocity_Control_xaxpy_o(int32_T n, real_T a, const real_T x
  [36], int32_T ix0, real_T y[6], int32_T iy0);
static real_T Parker_Velocity_Control_xdotc(int32_T n, const real_T x[36],
  int32_T ix0, const real_T y[36], int32_T iy0);
static void Parker_Velocity_Control_xaxpy(int32_T n, real_T a, int32_T ix0,
  real_T y[36], int32_T iy0);
static void Parker_Velocity_Control_xscal(real_T a, real_T x[36], int32_T ix0);
static void Parker_Velocity_Control_xswap(real_T x[36], int32_T ix0, int32_T iy0);
static void Parker_Velocity_Control_xrotg(real_T *a, real_T *b, real_T *c,
  real_T *s);
static void Parker_Velocity_Control_xrot(real_T x[36], int32_T ix0, int32_T iy0,
  real_T c, real_T s);
static void Parker_Velocity_Control_svd(const real_T A[36], real_T U[36], real_T
  s[6], real_T V[36]);
static void Parker_Velocity__rt_invd3x3_snf(const real_T u[9], real_T y[9]);
static void P_SystemCore_rele_h(const MoveArm_Speed_Parker_Velocity_T *obj);
static void P_SystemCore_dele_g(const MoveArm_Speed_Parker_Velocity_T *obj);
static void Parker_Veloci_matlabCodegenHa_g(MoveArm_Speed_Parker_Velocity_T *obj);
static void Parker_Vel_SystemCore_release_i(const
  codertarget_linux_blocks_Digi_T *obj);
static void Parker_Velo_SystemCore_delete_i(const
  codertarget_linux_blocks_Digi_T *obj);
static void matlabCodegenHandle_matlabCod_i(codertarget_linux_blocks_Digi_T *obj);
static void Parker_Ve_SystemCore_release_ir(const
  codertarget_linux_blocks_Digi_T *obj);
static void Parker_Vel_SystemCore_delete_ir(const
  codertarget_linux_blocks_Digi_T *obj);
static void matlabCodegenHandle_matlabCo_ir(codertarget_linux_blocks_Digi_T *obj);
static void Parker_Veloci_matlabCodegenHa_h(ReadArm_Position_Rates_Parker_T *obj);
static void Parker_Veloc_SystemCore_release(const
  InitializeForSpeed_Parker_Vel_T *obj);
static void Parker_Veloci_SystemCore_delete(const
  InitializeForSpeed_Parker_Vel_T *obj);
static void matlabCodegenHandle_matlabCodeg(InitializeForSpeed_Parker_Vel_T *obj);
static void Parker_Veloci_matlabCodegenHa_m(IdentifyPlatform_Parker_Veloc_T *obj);
static void Parker_V_SystemCore_release_iri(const
  raspi_internal_PWMBlock_Parke_T *obj);
static void Parker_Ve_SystemCore_delete_iri(const
  raspi_internal_PWMBlock_Parke_T *obj);
static void matlabCodegenHandle_matlabC_iri(raspi_internal_PWMBlock_Parke_T *obj);
static void SystemCore_release_iriacmixebgr(const
  codertarget_linux_blocks_Di_i_T *obj);
static void SystemCore_delete_iriacmixebgr(const codertarget_linux_blocks_Di_i_T
  *obj);
static void matlabCodegenHandl_iriacmixebgr(codertarget_linux_blocks_Di_i_T *obj);
static void SystemCore_releas_iriacmixebgrh(const
  codertarget_linux_blocks_Digi_T *obj);
static void SystemCore_delete_iriacmixebgrh(const
  codertarget_linux_blocks_Digi_T *obj);
static void matlabCodegenHand_iriacmixebgrh(codertarget_linux_blocks_Digi_T *obj);
static void SystemCore_rele_iriacmixebgrhtp(const
  codertarget_linux_blocks_Digi_T *obj);
static void SystemCore_dele_iriacmixebgrhtp(const
  codertarget_linux_blocks_Digi_T *obj);
static void matlabCodegenHa_iriacmixebgrhtp(codertarget_linux_blocks_Digi_T *obj);
static void rate_monotonic_scheduler(void);

//
// Set which subrates need to run this base step (base rate always runs).
// This function must be called prior to calling the model step function
// in order to "remember" which rates need to run this base step.  The
// buffering of events allows for overlapping preemption.
//
void Parker_Velocity_Control_SetEventsForThisBaseStep(boolean_T *eventFlags)
{
  // Task runs when its counter is zero, computed via rtmStepTask macro
  eventFlags[2] = ((boolean_T)rtmStepTask(Parker_Velocity_Control_M, 2));
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
  if (Parker_Velocity_Control_M->Timing.TaskCounters.TID[1] == 0) {
    Parker_Velocity_Control_M->Timing.RateInteraction.TID1_2 =
      (Parker_Velocity_Control_M->Timing.TaskCounters.TID[2] == 0);
  }

  // Compute which subrates run during the next base time step.  Subrates
  //  are an integer multiple of the base rate counter.  Therefore, the subtask
  //  counter is reset when it reaches its limit (zero means run).

  (Parker_Velocity_Control_M->Timing.TaskCounters.TID[2])++;
  if ((Parker_Velocity_Control_M->Timing.TaskCounters.TID[2]) > 1) {// Sample time: [0.1s, 0.0s] 
    Parker_Velocity_Control_M->Timing.TaskCounters.TID[2] = 0;
  }
}

//
// Output and update for atomic system:
//    '<S34>/MATLAB Function10'
//    '<S46>/MATLAB Function6'
//    '<S58>/MATLAB Function5'
//
void Parker_Vel_MATLABFunction10(const real_T rtu_PWMs[8],
  B_MATLABFunction10_Parker_Vel_T *localB)
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
    x[i] = (localB->PWMs[i] > Parker_Velocity_Control_P.thruster_count_threshold);
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
void Parker_Velo_MATLABFunction2(const real_T rtu_ThrustPer[8],
  B_MATLABFunction2_Parker_Velo_T *localB)
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
//
void Parker_CreateRotationMatrix(real_T rtu_Rz, B_CreateRotationMatrix_Parker_T *
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
void Park_CreateRotationMatrix_h(real_T rtu_Rz, B_CreateRotationMatrix_Park_i_T *
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
void Parker_Veloc_MATLABFunction(B_MATLABFunction_Parker_Veloc_T *localB)
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
    localB->v[i] = Parker_Velocity_Control_P.F_thrusters_BLUE[i] / 2.0;
  }

  memset(&localB->Mat2[0], 0, sizeof(real_T) << 6U);

  // '<S48>:1:27' H    = Mat1*Mat2;
  for (i = 0; i < 8; i++) {
    localB->Mat2[i + (i << 3)] = localB->v[i];
    localB->b[3 * i] = b[i];
    localB->b[3 * i + 1] = c[i];
    localB->b[3 * i + 2] = Parker_Velocity_Control_P.thruster_dist2CG_BLUE[i] /
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

//
// Output and update for atomic system:
//    '<S130>/Correct Dropouts'
//    '<S131>/Correct Dropouts'
//    '<S132>/Correct Dropouts'
//
void Parker_Velo_CorrectDropouts(real_T rtu_InputRaw, real_T rtu_InputRaw_f,
  B_CorrectDropouts_Parker_Velo_T *localB)
{
  // SignalConversion generated from: '<S133>/ SFunction '
  // MATLAB Function 'Manipulator Encoder Data/Change Behavior/Correct for dropouts/Correct Dropouts': '<S133>:1' 
  // '<S133>:1:3' if (InputRaw(1) < -3)
  if (rtu_InputRaw < -3.0) {
    // '<S133>:1:5' FixedInput = InputRaw(2);
    localB->FixedInput = rtu_InputRaw_f;
  } else {
    // '<S133>:1:15' else
    // '<S133>:1:17' FixedInput = InputRaw(1);
    localB->FixedInput = rtu_InputRaw;
  }

  // End of SignalConversion generated from: '<S133>/ SFunction '
}

//
// Output and update for action system:
//    '<S10>/Change BLUE Behavior'
//    '<S11>/Change BLUE Behavior'
//    '<S16>/Change BLUE Behavior'
//
void Parker_V_ChangeBLUEBehavior(P_ChangeBLUEBehavior_Parker_V_T *localP, real_T
  *rtd_BLUE_Fx, real_T *rtd_BLUE_Fy, real_T *rtd_BLUE_Tz, real_T
  *rtd_Float_State)
{
  // DataStoreWrite: '<S141>/BLUE_Fx' incorporates:
  //   Constant: '<S141>/Constant3'

  *rtd_BLUE_Fx = localP->Constant3_Value;

  // DataStoreWrite: '<S141>/BLUE_Fy' incorporates:
  //   Constant: '<S141>/Constant4'

  *rtd_BLUE_Fy = localP->Constant4_Value;

  // DataStoreWrite: '<S141>/BLUE_Tz' incorporates:
  //   Constant: '<S141>/Constant5'

  *rtd_BLUE_Tz = localP->Constant5_Value;

  // DataStoreWrite: '<S141>/Data Store Write4' incorporates:
  //   Constant: '<S141>/Puck State'

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
  //   Constant: '<S143>/Constant'

  if ((*rtd_WhoAmI == 2.0) || (Parker_Velocity_Control_P.simMode == 1.0)) {
    // Outputs for IfAction SubSystem: '<S10>/Change BLACK Behavior' incorporates:
    //   ActionPort: '<S140>/Action Port'

    // DataStoreWrite: '<S140>/BLACK_Fx' incorporates:
    //   Constant: '<S140>/Constant3'

    *rtd_BLACK_Fx = localP->Constant3_Value;

    // DataStoreWrite: '<S140>/BLACK_Fy' incorporates:
    //   Constant: '<S140>/Constant4'

    *rtd_BLACK_Fy = localP->Constant4_Value;

    // DataStoreWrite: '<S140>/BLACK_Tz' incorporates:
    //   Constant: '<S140>/Constant5'

    *rtd_BLACK_Tz = localP->Constant5_Value;

    // DataStoreWrite: '<S140>/Data Store Write4' incorporates:
    //   Constant: '<S140>/Puck State'

    *rtd_Float_State = localP->PuckState_Value;

    // End of Outputs for SubSystem: '<S10>/Change BLACK Behavior'
  }

  // End of If: '<S10>/This IF block determines whether or not to run the BLACK sim//exp' 

  // If: '<S10>/This IF block determines whether or not to run the BLUE sim//exp' incorporates:
  //   Constant: '<S143>/Constant'

  if ((*rtd_WhoAmI == 3.0) || (Parker_Velocity_Control_P.simMode == 1.0)) {
    // Outputs for IfAction SubSystem: '<S10>/Change BLUE Behavior' incorporates:
    //   ActionPort: '<S141>/Action Port'

    Parker_V_ChangeBLUEBehavior(&localP->ChangeBLUEBehavior, rtd_BLUE_Fx,
      rtd_BLUE_Fy, rtd_BLUE_Tz, rtd_Float_State);

    // End of Outputs for SubSystem: '<S10>/Change BLUE Behavior'
  }

  // End of If: '<S10>/This IF block determines whether or not to run the BLUE sim//exp' 

  // If: '<S10>/This IF block determines whether or not to run the RED sim//exp ' incorporates:
  //   Constant: '<S143>/Constant'

  if ((*rtd_WhoAmI == 1.0) || (Parker_Velocity_Control_P.simMode == 1.0)) {
    // Outputs for IfAction SubSystem: '<S10>/Change RED Behavior' incorporates:
    //   ActionPort: '<S142>/Action Port'

    // DataStoreWrite: '<S142>/RED_Fx' incorporates:
    //   Constant: '<S142>/Constant'

    *rtd_RED_Fx = localP->Constant_Value;

    // DataStoreWrite: '<S142>/RED_Fy' incorporates:
    //   Constant: '<S142>/Constant1'

    *rtd_RED_Fy = localP->Constant1_Value;

    // DataStoreWrite: '<S142>/RED_Tz' incorporates:
    //   Constant: '<S142>/Constant2'

    *rtd_RED_Tz = localP->Constant2_Value;

    // DataStoreWrite: '<S142>/RED_Tz_RW' incorporates:
    //   Constant: '<S142>/Constant3'

    *rtd_RED_Tz_RW = localP->Constant3_Value_h;

    // DataStoreWrite: '<S142>/Data Store Write' incorporates:
    //   Constant: '<S142>/Constant4'

    *rtd_RED_Tz_Shoulder = localP->Constant4_Value_g;

    // DataStoreWrite: '<S142>/Data Store Write1' incorporates:
    //   Constant: '<S142>/Constant5'

    *rtd_RED_Tz_Elbow = localP->Constant5_Value_e;

    // DataStoreWrite: '<S142>/Data Store Write2' incorporates:
    //   Constant: '<S142>/Constant6'

    *rtd_RED_Tz_Wrist = localP->Constant6_Value;

    // DataStoreWrite: '<S142>/Data Store Write4' incorporates:
    //   Constant: '<S142>/Puck State'

    *rtd_Float_State = localP->PuckState_Value_b;

    // End of Outputs for SubSystem: '<S10>/Change RED Behavior'
  }

  // End of If: '<S10>/This IF block determines whether or not to run the RED sim//exp ' 
}

//
// Output and update for atomic system:
//    '<S155>/MATLAB Function2'
//    '<S168>/MATLAB Function2'
//    '<S354>/MATLAB Function2'
//    '<S367>/MATLAB Function2'
//    '<S380>/MATLAB Function2'
//    '<S407>/MATLAB Function2'
//    '<S420>/MATLAB Function2'
//    '<S433>/MATLAB Function2'
//
void Parker_Ve_MATLABFunction2_o(real_T rtu_z, B_MATLABFunction2_Parker_Ve_n_T
  *localB)
{
  real_T Ox_tmp_tmp;
  real_T Ox_tmp_tmp_0;

  // MATLAB Function 'Phase #2:  Move to  Initial Position/Change BLACK Behavior/Hough Control/MATLAB Function2': '<S162>:1' 
  // '<S162>:1:6' Ox = [cos(z) sin(z); -sin(z) cos(z)]*[1;0];
  Ox_tmp_tmp = sin(rtu_z);
  Ox_tmp_tmp_0 = cos(rtu_z);
  localB->Ox[0] = 0.0;
  localB->Ox[0] += Ox_tmp_tmp_0;
  localB->Ox[0] += Ox_tmp_tmp * 0.0;
  localB->Ox[1] = 0.0;
  localB->Ox[1] += -Ox_tmp_tmp;
  localB->Ox[1] += Ox_tmp_tmp_0 * 0.0;

  // '<S162>:1:7' Oy = [cos(z) sin(z); -sin(z) cos(z)]*[0;1];
  localB->Oy[0] = 0.0;
  localB->Oy[0] += cos(rtu_z) * 0.0;
  localB->Oy[0] += Ox_tmp_tmp;
  localB->Oy[1] = 0.0;
  localB->Oy[1] += -sin(rtu_z) * 0.0;
  localB->Oy[1] += Ox_tmp_tmp_0;
}

//
// Output and update for atomic system:
//    '<S155>/MATLAB Function3'
//    '<S168>/MATLAB Function3'
//    '<S193>/MATLAB Function3'
//    '<S210>/MATLAB Function3'
//    '<S227>/MATLAB Function3'
//    '<S244>/MATLAB Function3'
//    '<S265>/MATLAB Function3'
//    '<S283>/MATLAB Function3'
//    '<S301>/MATLAB Function3'
//    '<S319>/MATLAB Function3'
//    ...
//
void Parker_Velo_MATLABFunction3(real_T rtu_z, B_MATLABFunction3_Parker_Velo_T
  *localB)
{
  real_T Ox_tmp;
  real_T Ox_tmp_0;

  // MATLAB Function 'Phase #2:  Move to  Initial Position/Change BLACK Behavior/Hough Control/MATLAB Function3': '<S163>:1' 
  // '<S163>:1:6' Ox = [cos(z) sin(z); -sin(z) cos(z)]*[1;0];
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
//    '<S155>/MATLAB Function4'
//    '<S168>/MATLAB Function4'
//    '<S193>/MATLAB Function4'
//    '<S210>/MATLAB Function4'
//    '<S227>/MATLAB Function4'
//    '<S244>/MATLAB Function4'
//    '<S265>/MATLAB Function4'
//    '<S283>/MATLAB Function4'
//    '<S301>/MATLAB Function4'
//    '<S319>/MATLAB Function4'
//    ...
//
void Parker_Velo_MATLABFunction4(const real_T rtu_Oy[2], const real_T rtu_e_in[2],
  B_MATLABFunction4_Parker_Velo_T *localB)
{
  real_T y;
  real_T scale;
  real_T absxk;
  real_T t;

  // MATLAB Function 'Phase #2:  Move to  Initial Position/Change BLACK Behavior/Hough Control/MATLAB Function4': '<S164>:1' 
  // '<S164>:1:3' a = sign(Oy'*e_in);
  // '<S164>:1:4' e_out = -a*norm(e_in);
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
// Output and update for atomic system:
//    '<S193>/MATLAB Function2'
//    '<S210>/MATLAB Function2'
//    '<S227>/MATLAB Function2'
//    '<S244>/MATLAB Function2'
//    '<S265>/MATLAB Function2'
//    '<S283>/MATLAB Function2'
//    '<S301>/MATLAB Function2'
//    '<S319>/MATLAB Function2'
//
void Parker_Ve_MATLABFunction2_l(real_T rtu_z, B_MATLABFunction2_Parker_Ve_c_T
  *localB)
{
  real_T Ox_tmp_tmp;
  real_T Ox_tmp_tmp_0;

  // MATLAB Function 'Phase #3: Experiment/Change BLACK Behavior/Sub-Phase #1/Hough Control/MATLAB Function2': '<S203>:1' 
  // '<S203>:1:6' Ox = [cos(z) sin(z); -sin(z) cos(z)]*[1;0];
  Ox_tmp_tmp = sin(rtu_z);
  Ox_tmp_tmp_0 = cos(rtu_z);
  localB->Ox[0] = 0.0;
  localB->Ox[0] += Ox_tmp_tmp_0;
  localB->Ox[0] += Ox_tmp_tmp * 0.0;
  localB->Ox[1] = 0.0;
  localB->Ox[1] += -Ox_tmp_tmp;
  localB->Ox[1] += Ox_tmp_tmp_0 * 0.0;

  // '<S203>:1:7' Oy = [cos(z) sin(z); -sin(z) cos(z)]*[0;1];
  localB->Oy[0] = 0.0;
  localB->Oy[0] += cos(rtu_z) * 0.0;
  localB->Oy[0] += Ox_tmp_tmp;
  localB->Oy[1] = 0.0;
  localB->Oy[1] += -sin(rtu_z) * 0.0;
  localB->Oy[1] += Ox_tmp_tmp_0;
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

//
// System initialize for action system:
//    '<S180>/Sub-Phase #1'
//    '<S180>/Sub-Phase #2 '
//    '<S180>/Sub-Phase #3 '
//    '<S180>/Sub-Phase #4'
//
void Parker_Veloc_SubPhase1_Init(B_SubPhase1_Parker_Velocity_C_T *localB,
  DW_SubPhase1_Parker_Velocity__T *localDW, P_SubPhase1_Parker_Velocity_C_T
  *localP)
{
  // InitializeConditions for Delay: '<S199>/Delay1'
  localDW->icLoad = 1U;

  // InitializeConditions for Delay: '<S201>/Delay1'
  localDW->icLoad_p = 1U;

  // InitializeConditions for Delay: '<S197>/Delay1'
  localDW->icLoad_pd = 1U;

  // SystemInitialize for IfAction SubSystem: '<S199>/Hold this value'
  // SystemInitialize for Outport: '<S200>/Out1'
  localB->In1_l = localP->Out1_Y0_n;

  // End of SystemInitialize for SubSystem: '<S199>/Hold this value'

  // SystemInitialize for IfAction SubSystem: '<S201>/Hold this value'
  // SystemInitialize for Outport: '<S202>/Out1'
  localB->In1 = localP->Out1_Y0_p;

  // End of SystemInitialize for SubSystem: '<S201>/Hold this value'

  // SystemInitialize for IfAction SubSystem: '<S197>/Hold this value'
  // SystemInitialize for Outport: '<S198>/Out1'
  localB->In1_j = localP->Out1_Y0;

  // End of SystemInitialize for SubSystem: '<S197>/Hold this value'
}

//
// Output and update for action system:
//    '<S180>/Sub-Phase #1'
//    '<S180>/Sub-Phase #2 '
//    '<S180>/Sub-Phase #3 '
//    '<S180>/Sub-Phase #4'
//
void Parker_Velocity_C_SubPhase1(B_SubPhase1_Parker_Velocity_C_T *localB,
  DW_SubPhase1_Parker_Velocity__T *localDW, P_SubPhase1_Parker_Velocity_C_T
  *localP, real_T *rtd_BLACK_Fx, real_T *rtd_BLACK_Fy, real_T *rtd_BLACK_Px,
  real_T *rtd_BLACK_Py, real_T *rtd_BLACK_Rz, real_T *rtd_BLACK_Tz, real_T
  *rtd_Float_State, real_T *rtd_Univ_Time)
{
  real_T rtb_Subtract_i;
  real_T rtb_Sum6_cq;
  real_T rtb_Sum6_nw;

  // Sum: '<S184>/Subtract' incorporates:
  //   Constant: '<S184>/Desired X-Position (BLACK)'

  rtb_Subtract_i = Parker_Velocity_Control_P.xLength / 2.0 - *rtd_BLACK_Px;

  // Delay: '<S199>/Delay1'
  if (localDW->icLoad != 0) {
    localDW->Delay1_DSTATE = rtb_Subtract_i;
  }

  // Sum: '<S199>/Sum6' incorporates:
  //   Delay: '<S199>/Delay1'

  rtb_Sum6_cq = rtb_Subtract_i - localDW->Delay1_DSTATE;

  // If: '<S199>/if we went through a "step"' incorporates:
  //   Inport: '<S200>/In1'

  if (rtb_Sum6_cq != 0.0) {
    // Outputs for IfAction SubSystem: '<S199>/Hold this value' incorporates:
    //   ActionPort: '<S200>/Action Port'

    localB->In1_l = rtb_Sum6_cq;

    // End of Outputs for SubSystem: '<S199>/Hold this value'
  }

  // End of If: '<S199>/if we went through a "step"'

  // Sum: '<S191>/Sum3' incorporates:
  //   Gain: '<S191>/kd_xb'
  //   Gain: '<S191>/kp_xb'
  //   Gain: '<S199>/divide by delta T'

  *rtd_BLACK_Fx = 1.0 / Parker_Velocity_Control_P.baseRate * localB->In1_l *
    Parker_Velocity_Control_P.Kd_xb + Parker_Velocity_Control_P.Kp_xb *
    rtb_Subtract_i;

  // Sum: '<S184>/Subtract1' incorporates:
  //   Constant: '<S184>/Desired Y-Position (BLACK)'

  rtb_Sum6_cq = Parker_Velocity_Control_P.yLength / 2.0 - *rtd_BLACK_Py;

  // Delay: '<S201>/Delay1'
  if (localDW->icLoad_p != 0) {
    localDW->Delay1_DSTATE_j = rtb_Sum6_cq;
  }

  // Sum: '<S201>/Sum6' incorporates:
  //   Delay: '<S201>/Delay1'

  rtb_Sum6_nw = rtb_Sum6_cq - localDW->Delay1_DSTATE_j;

  // If: '<S201>/if we went through a "step"' incorporates:
  //   Inport: '<S202>/In1'

  if (rtb_Sum6_nw != 0.0) {
    // Outputs for IfAction SubSystem: '<S201>/Hold this value' incorporates:
    //   ActionPort: '<S202>/Action Port'

    localB->In1 = rtb_Sum6_nw;

    // End of Outputs for SubSystem: '<S201>/Hold this value'
  }

  // End of If: '<S201>/if we went through a "step"'

  // Sum: '<S192>/Sum3' incorporates:
  //   Gain: '<S192>/kd_yb'
  //   Gain: '<S192>/kp_yb'
  //   Gain: '<S201>/divide by delta T'

  *rtd_BLACK_Fy = 1.0 / Parker_Velocity_Control_P.baseRate * localB->In1 *
    Parker_Velocity_Control_P.Kd_yb + Parker_Velocity_Control_P.Kp_yb *
    rtb_Sum6_cq;

  // Product: '<S189>/Product' incorporates:
  //   Constant: '<S189>/Desired Rate (BLACK)'
  //   Constant: '<S194>/Constant1'
  //   Sum: '<S194>/Subtract'

  rtb_Sum6_nw = (*rtd_Univ_Time - Parker_Velocity_Control_P.Phase2_End) *
    localP->DesiredRateBLACK_Value;

  // MATLAB Function: '<S193>/MATLAB Function2' incorporates:
  //   MATLAB Function: '<S195>/MATLAB Function'

  // MATLAB Function 'Phase #3: Experiment/Change BLACK Behavior/Sub-Phase #1/BLACK_RzD1/Wrap1/MATLAB Function': '<S196>:1' 
  // '<S196>:1:3' wrappedAngle = atan2(sin(u),cos(u));
  Parker_Ve_MATLABFunction2_l(rt_atan2d_snf(sin(rtb_Sum6_nw), cos(rtb_Sum6_nw)),
    &localB->sf_MATLABFunction2);

  // MATLAB Function: '<S193>/MATLAB Function3'
  Parker_Velo_MATLABFunction3(*rtd_BLACK_Rz, &localB->sf_MATLABFunction3);

  // Sum: '<S193>/Subtract2'
  localB->rtb_Subtract2_d_m[0] = localB->sf_MATLABFunction2.Ox[0] -
    localB->sf_MATLABFunction3.Ox[0];
  localB->rtb_Subtract2_d_m[1] = localB->sf_MATLABFunction2.Ox[1] -
    localB->sf_MATLABFunction3.Ox[1];

  // MATLAB Function: '<S193>/MATLAB Function4'
  Parker_Velo_MATLABFunction4(localB->sf_MATLABFunction2.Oy,
    localB->rtb_Subtract2_d_m, &localB->sf_MATLABFunction4);

  // Delay: '<S197>/Delay1'
  if (localDW->icLoad_pd != 0) {
    localDW->Delay1_DSTATE_m = localB->sf_MATLABFunction4.e_out;
  }

  // Sum: '<S197>/Sum6' incorporates:
  //   Delay: '<S197>/Delay1'

  rtb_Sum6_nw = localB->sf_MATLABFunction4.e_out - localDW->Delay1_DSTATE_m;

  // If: '<S197>/if we went through a "step"' incorporates:
  //   Inport: '<S198>/In1'

  if (rtb_Sum6_nw != 0.0) {
    // Outputs for IfAction SubSystem: '<S197>/Hold this value' incorporates:
    //   ActionPort: '<S198>/Action Port'

    localB->In1_j = rtb_Sum6_nw;

    // End of Outputs for SubSystem: '<S197>/Hold this value'
  }

  // End of If: '<S197>/if we went through a "step"'

  // Sum: '<S190>/Sum3' incorporates:
  //   Gain: '<S190>/kd_tb'
  //   Gain: '<S190>/kp_tb'
  //   Gain: '<S197>/divide by delta T'

  *rtd_BLACK_Tz = 1.0 / Parker_Velocity_Control_P.baseRate * localB->In1_j *
    Parker_Velocity_Control_P.Kd_tb + Parker_Velocity_Control_P.Kp_tb *
    localB->sf_MATLABFunction4.e_out;

  // DataStoreWrite: '<S184>/Data Store Write3' incorporates:
  //   Constant: '<S184>/Puck State'

  *rtd_Float_State = localP->PuckState_Value;

  // Update for Delay: '<S199>/Delay1'
  localDW->icLoad = 0U;
  localDW->Delay1_DSTATE = rtb_Subtract_i;

  // Update for Delay: '<S201>/Delay1'
  localDW->icLoad_p = 0U;
  localDW->Delay1_DSTATE_j = rtb_Sum6_cq;

  // Update for Delay: '<S197>/Delay1'
  localDW->icLoad_pd = 0U;
  localDW->Delay1_DSTATE_m = localB->sf_MATLABFunction4.e_out;
}

//
// System initialize for action system:
//    '<S181>/Sub-Phase #1'
//    '<S181>/Sub-Phase #2 '
//    '<S181>/Sub-Phase #3 '
//    '<S181>/Sub-Phase #4'
//
void Parker_Vel_SubPhase1_n_Init(B_SubPhase1_Parker_Velocity_g_T *localB,
  DW_SubPhase1_Parker_Velocit_l_T *localDW, P_SubPhase1_Parker_Velocity_k_T
  *localP)
{
  // InitializeConditions for Delay: '<S272>/Delay1'
  localDW->icLoad = 1U;

  // InitializeConditions for Delay: '<S274>/Delay1'
  localDW->icLoad_p = 1U;

  // InitializeConditions for Delay: '<S270>/Delay1'
  localDW->icLoad_m = 1U;

  // SystemInitialize for IfAction SubSystem: '<S272>/Hold this value'
  // SystemInitialize for Outport: '<S273>/Out1'
  localB->In1_j = localP->Out1_Y0_g;

  // End of SystemInitialize for SubSystem: '<S272>/Hold this value'

  // SystemInitialize for IfAction SubSystem: '<S274>/Hold this value'
  // SystemInitialize for Outport: '<S275>/Out1'
  localB->In1 = localP->Out1_Y0_p;

  // End of SystemInitialize for SubSystem: '<S274>/Hold this value'

  // SystemInitialize for IfAction SubSystem: '<S270>/Hold this value'
  // SystemInitialize for Outport: '<S271>/Out1'
  localB->In1_m = localP->Out1_Y0;

  // End of SystemInitialize for SubSystem: '<S270>/Hold this value'
}

//
// Output and update for action system:
//    '<S181>/Sub-Phase #1'
//    '<S181>/Sub-Phase #2 '
//    '<S181>/Sub-Phase #3 '
//    '<S181>/Sub-Phase #4'
//
void Parker_Velocity_SubPhase1_a(B_SubPhase1_Parker_Velocity_g_T *localB,
  DW_SubPhase1_Parker_Velocit_l_T *localDW, P_SubPhase1_Parker_Velocity_k_T
  *localP, real_T *rtd_BLACK_Px, real_T *rtd_BLACK_Py, real_T *rtd_BLUE_Fx,
  real_T *rtd_BLUE_Fy, real_T *rtd_BLUE_Px, real_T *rtd_BLUE_Py, real_T
  *rtd_BLUE_Rz, real_T *rtd_BLUE_Tz, real_T *rtd_Float_State, real_T
  *rtd_Univ_Time)
{
  real_T rtb_Subtract_c;
  real_T rtb_Sum6_l4;
  real_T rtb_Subtract3;

  // Sum: '<S269>/Subtract3' incorporates:
  //   Constant: '<S269>/Constant2'

  rtb_Subtract3 = *rtd_Univ_Time - Parker_Velocity_Control_P.Phase2_End;

  // Sum: '<S257>/Subtract' incorporates:
  //   MATLAB Function: '<S257>/MATLAB Function2'

  // MATLAB Function 'Phase #3: Experiment/Change BLUE Behavior/Sub-Phase #1/MATLAB Function2': '<S266>:1' 
  // '<S266>:1:3' x = -0.85*cos((0.03490659)*t) + xLength/2;
  rtb_Subtract_c = (cos(0.03490659 * rtb_Subtract3) * -0.85 +
                    Parker_Velocity_Control_P.xLength / 2.0) - *rtd_BLUE_Px;

  // Delay: '<S272>/Delay1'
  if (localDW->icLoad != 0) {
    localDW->Delay1_DSTATE = rtb_Subtract_c;
  }

  // Sum: '<S272>/Sum6' incorporates:
  //   Delay: '<S272>/Delay1'

  rtb_Sum6_l4 = rtb_Subtract_c - localDW->Delay1_DSTATE;

  // If: '<S272>/if we went through a "step"' incorporates:
  //   Inport: '<S273>/In1'

  if (rtb_Sum6_l4 != 0.0) {
    // Outputs for IfAction SubSystem: '<S272>/Hold this value' incorporates:
    //   ActionPort: '<S273>/Action Port'

    localB->In1_j = rtb_Sum6_l4;

    // End of Outputs for SubSystem: '<S272>/Hold this value'
  }

  // End of If: '<S272>/if we went through a "step"'

  // Sum: '<S263>/Sum3' incorporates:
  //   Gain: '<S263>/kd_xb'
  //   Gain: '<S263>/kp_xb'
  //   Gain: '<S272>/divide by delta T'

  *rtd_BLUE_Fx = 1.0 / Parker_Velocity_Control_P.baseRate * localB->In1_j *
    Parker_Velocity_Control_P.Kd_xblue + Parker_Velocity_Control_P.Kp_xblue *
    rtb_Subtract_c;

  // Sum: '<S257>/Subtract1' incorporates:
  //   MATLAB Function: '<S257>/MATLAB Function3'

  // MATLAB Function 'Phase #3: Experiment/Change BLUE Behavior/Sub-Phase #1/MATLAB Function3': '<S267>:1' 
  // '<S267>:1:3' y = -0.85*sin((0.03490659)*t) + yLength/2;
  rtb_Subtract3 = (sin(0.03490659 * rtb_Subtract3) * -0.85 +
                   Parker_Velocity_Control_P.yLength / 2.0) - *rtd_BLUE_Py;

  // Delay: '<S274>/Delay1'
  if (localDW->icLoad_p != 0) {
    localDW->Delay1_DSTATE_j = rtb_Subtract3;
  }

  // Sum: '<S274>/Sum6' incorporates:
  //   Delay: '<S274>/Delay1'

  rtb_Sum6_l4 = rtb_Subtract3 - localDW->Delay1_DSTATE_j;

  // If: '<S274>/if we went through a "step"' incorporates:
  //   Inport: '<S275>/In1'

  if (rtb_Sum6_l4 != 0.0) {
    // Outputs for IfAction SubSystem: '<S274>/Hold this value' incorporates:
    //   ActionPort: '<S275>/Action Port'

    localB->In1 = rtb_Sum6_l4;

    // End of Outputs for SubSystem: '<S274>/Hold this value'
  }

  // End of If: '<S274>/if we went through a "step"'

  // Sum: '<S264>/Sum3' incorporates:
  //   Gain: '<S264>/kd_yb'
  //   Gain: '<S264>/kp_yb'
  //   Gain: '<S274>/divide by delta T'

  *rtd_BLUE_Fy = 1.0 / Parker_Velocity_Control_P.baseRate * localB->In1 *
    Parker_Velocity_Control_P.Kd_yblue + Parker_Velocity_Control_P.Kp_yblue *
    rtb_Subtract3;

  // MATLAB Function: '<S265>/MATLAB Function2' incorporates:
  //   Constant: '<S268>/Constant'
  //   Sum: '<S268>/Subtract'
  //   Sum: '<S268>/Subtract1'
  //   Sum: '<S268>/Sum'
  //   Trigonometry: '<S268>/Atan2'

  Parker_Ve_MATLABFunction2_l(rt_atan2d_snf(*rtd_BLACK_Py - *rtd_BLUE_Py,
    *rtd_BLACK_Px - *rtd_BLUE_Px) + localP->Constant_Value,
    &localB->sf_MATLABFunction2);

  // MATLAB Function: '<S265>/MATLAB Function3'
  Parker_Velo_MATLABFunction3(*rtd_BLUE_Rz, &localB->sf_MATLABFunction3);

  // Sum: '<S265>/Subtract2'
  localB->rtb_Subtract2_e_m[0] = localB->sf_MATLABFunction2.Ox[0] -
    localB->sf_MATLABFunction3.Ox[0];
  localB->rtb_Subtract2_e_m[1] = localB->sf_MATLABFunction2.Ox[1] -
    localB->sf_MATLABFunction3.Ox[1];

  // MATLAB Function: '<S265>/MATLAB Function4'
  Parker_Velo_MATLABFunction4(localB->sf_MATLABFunction2.Oy,
    localB->rtb_Subtract2_e_m, &localB->sf_MATLABFunction4);

  // Delay: '<S270>/Delay1'
  if (localDW->icLoad_m != 0) {
    localDW->Delay1_DSTATE_o = localB->sf_MATLABFunction4.e_out;
  }

  // Sum: '<S270>/Sum6' incorporates:
  //   Delay: '<S270>/Delay1'

  rtb_Sum6_l4 = localB->sf_MATLABFunction4.e_out - localDW->Delay1_DSTATE_o;

  // If: '<S270>/if we went through a "step"' incorporates:
  //   Inport: '<S271>/In1'

  if (rtb_Sum6_l4 != 0.0) {
    // Outputs for IfAction SubSystem: '<S270>/Hold this value' incorporates:
    //   ActionPort: '<S271>/Action Port'

    localB->In1_m = rtb_Sum6_l4;

    // End of Outputs for SubSystem: '<S270>/Hold this value'
  }

  // End of If: '<S270>/if we went through a "step"'

  // Sum: '<S262>/Sum3' incorporates:
  //   Gain: '<S262>/kd_tb'
  //   Gain: '<S262>/kp_tb'
  //   Gain: '<S270>/divide by delta T'

  *rtd_BLUE_Tz = 1.0 / Parker_Velocity_Control_P.baseRate * localB->In1_m *
    Parker_Velocity_Control_P.Kd_tblue + Parker_Velocity_Control_P.Kp_tblue *
    localB->sf_MATLABFunction4.e_out;

  // DataStoreWrite: '<S257>/Data Store Write1' incorporates:
  //   Constant: '<S257>/Puck State'

  *rtd_Float_State = localP->PuckState_Value;

  // Update for Delay: '<S272>/Delay1'
  localDW->icLoad = 0U;
  localDW->Delay1_DSTATE = rtb_Subtract_c;

  // Update for Delay: '<S274>/Delay1'
  localDW->icLoad_p = 0U;
  localDW->Delay1_DSTATE_j = rtb_Subtract3;

  // Update for Delay: '<S270>/Delay1'
  localDW->icLoad_m = 0U;
  localDW->Delay1_DSTATE_o = localB->sf_MATLABFunction4.e_out;
}

//
// Output and update for action system:
//    '<S182>/Sub-Phase #1'
//    '<S182>/Sub-Phase #2 '
//
void Parker_Velocity_SubPhase1_j(P_SubPhase1_Parker_Velocity_m_T *localP, real_T
  *rtd_Float_State, real_T *rtd_RED_Tz_Elbow, real_T *rtd_RED_Tz_RW, real_T
  *rtd_RED_Tz_Shoulder, real_T *rtd_RED_Tz_Wrist)
{
  // DataStoreWrite: '<S334>/RED_Tz_RW' incorporates:
  //   Constant: '<S334>/Constant1'

  *rtd_RED_Tz_RW = localP->Constant1_Value_o;

  // DataStoreWrite: '<S334>/Data Store Write3' incorporates:
  //   Constant: '<S334>/Puck State'

  *rtd_Float_State = localP->PuckState_Value;

  // If: '<S334>/If' incorporates:
  //   Constant: '<S334>/Constant4'

  if ((Parker_Velocity_Control_P.platformSelection == 4.0) ||
      (Parker_Velocity_Control_P.platformSelection == 5.0) ||
      (Parker_Velocity_Control_P.platformSelection == 10.0) ||
      (Parker_Velocity_Control_P.platformSelection == 11.0)) {
    // Outputs for IfAction SubSystem: '<S334>/RED+ARM' incorporates:
    //   ActionPort: '<S340>/Action Port'

    // DataStoreWrite: '<S340>/ARM_EL_Tz' incorporates:
    //   Constant: '<S340>/Constant1'

    *rtd_RED_Tz_Elbow = localP->Constant1_Value;

    // DataStoreWrite: '<S340>/ARM_SH_Tz' incorporates:
    //   Constant: '<S340>/Constant1'

    *rtd_RED_Tz_Shoulder = localP->Constant1_Value;

    // DataStoreWrite: '<S340>/ARM_WR_Tz' incorporates:
    //   Constant: '<S340>/Constant1'

    *rtd_RED_Tz_Wrist = localP->Constant1_Value;

    // End of Outputs for SubSystem: '<S334>/RED+ARM'
  } else {
    // Outputs for IfAction SubSystem: '<S334>/RED' incorporates:
    //   ActionPort: '<S339>/Action Port'

    // DataStoreWrite: '<S339>/RED_Fx1' incorporates:
    //   Constant: '<S339>/Constant7'

    *rtd_RED_Tz_Shoulder = localP->Constant7_Value;

    // DataStoreWrite: '<S339>/RED_Fy1' incorporates:
    //   Constant: '<S339>/Constant7'

    *rtd_RED_Tz_Elbow = localP->Constant7_Value;

    // DataStoreWrite: '<S339>/RED_Tz1' incorporates:
    //   Constant: '<S339>/Constant7'

    *rtd_RED_Tz_Wrist = localP->Constant7_Value;

    // End of Outputs for SubSystem: '<S334>/RED'
  }

  // End of If: '<S334>/If'
}

//
// System initialize for action system:
//    '<S14>/Change BLUE Behavior'
//    '<S15>/Change BLUE Behavior'
//
void P_ChangeBLUEBehavior_a_Init(B_ChangeBLUEBehavior_Parker_k_T *localB,
  DW_ChangeBLUEBehavior_Parke_i_T *localDW, P_ChangeBLUEBehavior_Parker_c_T
  *localP)
{
  // InitializeConditions for Delay: '<S370>/Delay1'
  localDW->icLoad = 1U;

  // InitializeConditions for Delay: '<S372>/Delay1'
  localDW->icLoad_m = 1U;

  // InitializeConditions for Delay: '<S368>/Delay1'
  localDW->icLoad_p = 1U;

  // SystemInitialize for IfAction SubSystem: '<S370>/Hold this value'
  // SystemInitialize for Outport: '<S371>/Out1'
  localB->In1_l = localP->Out1_Y0_n;

  // End of SystemInitialize for SubSystem: '<S370>/Hold this value'

  // SystemInitialize for IfAction SubSystem: '<S372>/Hold this value'
  // SystemInitialize for Outport: '<S373>/Out1'
  localB->In1 = localP->Out1_Y0_p;

  // End of SystemInitialize for SubSystem: '<S372>/Hold this value'

  // SystemInitialize for IfAction SubSystem: '<S368>/Hold this value'
  // SystemInitialize for Outport: '<S369>/Out1'
  localB->In1_p = localP->Out1_Y0;

  // End of SystemInitialize for SubSystem: '<S368>/Hold this value'
}

//
// Output and update for action system:
//    '<S14>/Change BLUE Behavior'
//    '<S15>/Change BLUE Behavior'
//
void Parker_ChangeBLUEBehavior_g(B_ChangeBLUEBehavior_Parker_k_T *localB,
  DW_ChangeBLUEBehavior_Parke_i_T *localDW, P_ChangeBLUEBehavior_Parker_c_T
  *localP, real_T *rtd_BLUE_Fx, real_T *rtd_BLUE_Fy, real_T *rtd_BLUE_Px, real_T
  *rtd_BLUE_Py, real_T *rtd_BLUE_Rz, real_T *rtd_BLUE_Tz, real_T
  *rtd_Float_State)
{
  real_T rtb_Subtract_e;
  real_T rtb_Sum6_ju;
  real_T rtb_Sum6_ad;

  // Sum: '<S348>/Subtract' incorporates:
  //   Constant: '<S348>/Desired Px (BLUE)'

  rtb_Subtract_e = Parker_Velocity_Control_P.home_states_BLUE[0] - *rtd_BLUE_Px;

  // Delay: '<S370>/Delay1'
  if (localDW->icLoad != 0) {
    localDW->Delay1_DSTATE = rtb_Subtract_e;
  }

  // Sum: '<S370>/Sum6' incorporates:
  //   Delay: '<S370>/Delay1'

  rtb_Sum6_ju = rtb_Subtract_e - localDW->Delay1_DSTATE;

  // If: '<S370>/if we went through a "step"' incorporates:
  //   Inport: '<S371>/In1'

  if (rtb_Sum6_ju != 0.0) {
    // Outputs for IfAction SubSystem: '<S370>/Hold this value' incorporates:
    //   ActionPort: '<S371>/Action Port'

    localB->In1_l = rtb_Sum6_ju;

    // End of Outputs for SubSystem: '<S370>/Hold this value'
  }

  // End of If: '<S370>/if we went through a "step"'

  // Sum: '<S365>/Sum3' incorporates:
  //   Gain: '<S365>/kd_xb'
  //   Gain: '<S365>/kp_xb'
  //   Gain: '<S370>/divide by delta T'

  *rtd_BLUE_Fx = 1.0 / Parker_Velocity_Control_P.baseRate * localB->In1_l *
    Parker_Velocity_Control_P.Kd_xblue + Parker_Velocity_Control_P.Kp_xblue *
    rtb_Subtract_e;

  // Sum: '<S348>/Subtract1' incorporates:
  //   Constant: '<S348>/Desired Py (BLUE)'

  rtb_Sum6_ju = Parker_Velocity_Control_P.home_states_BLUE[1] - *rtd_BLUE_Py;

  // Delay: '<S372>/Delay1'
  if (localDW->icLoad_m != 0) {
    localDW->Delay1_DSTATE_j = rtb_Sum6_ju;
  }

  // Sum: '<S372>/Sum6' incorporates:
  //   Delay: '<S372>/Delay1'

  rtb_Sum6_ad = rtb_Sum6_ju - localDW->Delay1_DSTATE_j;

  // If: '<S372>/if we went through a "step"' incorporates:
  //   Inport: '<S373>/In1'

  if (rtb_Sum6_ad != 0.0) {
    // Outputs for IfAction SubSystem: '<S372>/Hold this value' incorporates:
    //   ActionPort: '<S373>/Action Port'

    localB->In1 = rtb_Sum6_ad;

    // End of Outputs for SubSystem: '<S372>/Hold this value'
  }

  // End of If: '<S372>/if we went through a "step"'

  // Sum: '<S366>/Sum3' incorporates:
  //   Gain: '<S366>/kd_yb'
  //   Gain: '<S366>/kp_yb'
  //   Gain: '<S372>/divide by delta T'

  *rtd_BLUE_Fy = 1.0 / Parker_Velocity_Control_P.baseRate * localB->In1 *
    Parker_Velocity_Control_P.Kd_yblue + Parker_Velocity_Control_P.Kp_yblue *
    rtb_Sum6_ju;

  // MATLAB Function: '<S367>/MATLAB Function2' incorporates:
  //   Constant: '<S348>/Desired Attitude (BLUE)'

  Parker_Ve_MATLABFunction2_o(Parker_Velocity_Control_P.home_states_BLUE[2],
    &localB->sf_MATLABFunction2);

  // MATLAB Function: '<S367>/MATLAB Function3'
  Parker_Velo_MATLABFunction3(*rtd_BLUE_Rz, &localB->sf_MATLABFunction3);

  // Sum: '<S367>/Subtract2'
  localB->rtb_Subtract2_b_m[0] = localB->sf_MATLABFunction2.Ox[0] -
    localB->sf_MATLABFunction3.Ox[0];
  localB->rtb_Subtract2_b_m[1] = localB->sf_MATLABFunction2.Ox[1] -
    localB->sf_MATLABFunction3.Ox[1];

  // MATLAB Function: '<S367>/MATLAB Function4'
  Parker_Velo_MATLABFunction4(localB->sf_MATLABFunction2.Oy,
    localB->rtb_Subtract2_b_m, &localB->sf_MATLABFunction4);

  // Delay: '<S368>/Delay1'
  if (localDW->icLoad_p != 0) {
    localDW->Delay1_DSTATE_n = localB->sf_MATLABFunction4.e_out;
  }

  // Sum: '<S368>/Sum6' incorporates:
  //   Delay: '<S368>/Delay1'

  rtb_Sum6_ad = localB->sf_MATLABFunction4.e_out - localDW->Delay1_DSTATE_n;

  // If: '<S368>/if we went through a "step"' incorporates:
  //   Inport: '<S369>/In1'

  if (rtb_Sum6_ad != 0.0) {
    // Outputs for IfAction SubSystem: '<S368>/Hold this value' incorporates:
    //   ActionPort: '<S369>/Action Port'

    localB->In1_p = rtb_Sum6_ad;

    // End of Outputs for SubSystem: '<S368>/Hold this value'
  }

  // End of If: '<S368>/if we went through a "step"'

  // Sum: '<S364>/Sum3' incorporates:
  //   Gain: '<S364>/kd_tb'
  //   Gain: '<S364>/kp_tb'
  //   Gain: '<S368>/divide by delta T'

  *rtd_BLUE_Tz = 1.0 / Parker_Velocity_Control_P.baseRate * localB->In1_p *
    Parker_Velocity_Control_P.Kd_tblue + Parker_Velocity_Control_P.Kp_tblue *
    localB->sf_MATLABFunction4.e_out;

  // DataStoreWrite: '<S348>/Data Store Write3' incorporates:
  //   Constant: '<S348>/Puck State'

  *rtd_Float_State = localP->PuckState_Value;

  // Update for Delay: '<S370>/Delay1'
  localDW->icLoad = 0U;
  localDW->Delay1_DSTATE = rtb_Subtract_e;

  // Update for Delay: '<S372>/Delay1'
  localDW->icLoad_m = 0U;
  localDW->Delay1_DSTATE_j = rtb_Sum6_ju;

  // Update for Delay: '<S368>/Delay1'
  localDW->icLoad_p = 0U;
  localDW->Delay1_DSTATE_n = localB->sf_MATLABFunction4.e_out;
}

//
// Output and update for action system:
//    '<S349>/RED+ARM'
//    '<S402>/RED+ARM'
//
void Parker_Velocity_Cont_REDARM(B_REDARM_Parker_Velocity_Cont_T *localB,
  P_REDARM_Parker_Velocity_Cont_T *localP, real_T *rtd_RED_RzD_Elbow, real_T
  *rtd_RED_RzD_Shoulder, real_T *rtd_RED_RzD_Wrist, real_T *rtd_RED_Rz_Elbow,
  real_T *rtd_RED_Rz_Shoulder, real_T *rtd_RED_Rz_Wrist, real_T
  *rtd_RED_Tz_Elbow, real_T *rtd_RED_Tz_Shoulder, real_T *rtd_RED_Tz_Wrist)
{
  real_T t114;
  real_T rtb_y_d_tmp;
  real_T rtb_y_d_tmp_0;
  int32_T i;
  int32_T i_0;
  real_T l1_tmp;
  real_T l2_tmp;
  real_T l3_tmp;
  int32_T tmp;

  // MATLAB Function: '<S392>/End-Effector Position' incorporates:
  //   MATLAB Function: '<S396>/MATLAB Function'

  // MATLAB Function 'Phase #4:  Return Home/Change RED Behavior/RED+ARM/Transpose Jacobian Controller/End-Effector Position': '<S394>:1' 
  // '<S394>:1:4' phi= model_param(7);
  // '<S394>:1:5' b0= model_param(8);
  // '<S394>:1:6' a1= model_param(9);
  // '<S394>:1:7' b1= model_param(10);
  // '<S394>:1:8' a2= model_param(11);
  // '<S394>:1:9' b2= model_param(12);
  // '<S394>:1:10' a3= model_param(13);
  // '<S394>:1:11' b3= model_param(14);
  // '<S394>:1:13' l1     = a1+b1;
  l1_tmp = Parker_Velocity_Control_P.model_param[8] +
    Parker_Velocity_Control_P.model_param[9];

  // '<S394>:1:14' l2     = a2+b2;
  l2_tmp = Parker_Velocity_Control_P.model_param[10] +
    Parker_Velocity_Control_P.model_param[11];

  // '<S394>:1:15' l3     = a3+b3;
  l3_tmp = Parker_Velocity_Control_P.model_param[12] +
    Parker_Velocity_Control_P.model_param[13];

  // '<S394>:1:18' x0 = q(1);
  // '<S394>:1:19' y0 = q(2);
  // '<S394>:1:20' q0 = q(3);
  // '<S394>:1:21' q1 = q(4);
  // '<S394>:1:22' q2 = q(5);
  // '<S394>:1:23' q3 = q(6);
  // '<S394>:1:26' y = [ (b0)*cos(phi) + (l1)*cos(pi/2+q1) + (l2)*cos(pi/2+q1+q2) + (l3)*cos(pi/2+q1+q2+q3),... 
  // '<S394>:1:27'       (b0)*sin(phi) + (l1)*sin(pi/2+q1) + (l2)*sin(pi/2+q1+q2) + (l3)*sin(pi/2+q1+q2+q3)]; 
  rtb_y_d_tmp = sin(Parker_Velocity_Control_P.model_param[6]);
  rtb_y_d_tmp_0 = cos(Parker_Velocity_Control_P.model_param[6]);

  // MATLAB Function: '<S396>/Inertia Function '
  // MATLAB Function 'Phase #4:  Return Home/Change RED Behavior/RED+ARM/Transpose Jacobian Controller/Transpose Generalized Jacobian /Inertia Function ': '<S398>:1' 
  // '<S398>:1:8' q0 = x(3);
  // '<S398>:1:9' q1 = x(4);
  // '<S398>:1:10' q2 = x(5);
  // '<S398>:1:11' q3 = x(6);
  // '<S398>:1:13' m0= model_param(1);
  // '<S398>:1:14' I0= model_param(2);
  // '<S398>:1:15' phi= model_param(7);
  // '<S398>:1:16' b0= model_param(8);
  // '<S398>:1:17' a1= model_param(9);
  // '<S398>:1:18' b1= model_param(10);
  // '<S398>:1:19' a2= model_param(11);
  // '<S398>:1:20' b2= model_param(12);
  // '<S398>:1:21' a3= model_param(13);
  // '<S398>:1:22' b3= model_param(14);
  // '<S398>:1:23' m1= model_param(15);
  // '<S398>:1:24' m2= model_param(16);
  // '<S398>:1:25' m3= model_param(17);
  // '<S398>:1:26' I1= model_param(18);
  // '<S398>:1:27' I2= model_param(19);
  // '<S398>:1:28' I3= model_param(20);
  // '<S398>:1:30' t2 = cos(q2);
  localB->t2 = cos(*rtd_RED_Rz_Elbow);

  // '<S398>:1:31' t3 = cos(q3);
  localB->t3 = cos(*rtd_RED_Rz_Wrist);

  // '<S398>:1:32' t4 = a1+b1;
  // '<S398>:1:33' t5 = a2+b2;
  // '<S398>:1:34' t6 = a1.*m1;
  localB->t6 = Parker_Velocity_Control_P.model_param[8] *
    Parker_Velocity_Control_P.model_param[14];

  // '<S398>:1:35' t7 = a2.*m2;
  localB->t7 = Parker_Velocity_Control_P.model_param[10] *
    Parker_Velocity_Control_P.model_param[15];

  // '<S398>:1:36' t8 = a2.*m3;
  localB->t8 = Parker_Velocity_Control_P.model_param[10] *
    Parker_Velocity_Control_P.model_param[16];

  // '<S398>:1:37' t9 = a3.*m3;
  localB->t9 = Parker_Velocity_Control_P.model_param[12] *
    Parker_Velocity_Control_P.model_param[16];

  // '<S398>:1:38' t10 = b0.*m1;
  // '<S398>:1:39' t11 = b0.*m2;
  localB->t11 = Parker_Velocity_Control_P.model_param[7] *
    Parker_Velocity_Control_P.model_param[15];

  // '<S398>:1:40' t12 = b0.*m3;
  localB->t12 = Parker_Velocity_Control_P.model_param[7] *
    Parker_Velocity_Control_P.model_param[16];

  // '<S398>:1:41' t13 = b2.*m3;
  localB->t13 = Parker_Velocity_Control_P.model_param[11] *
    Parker_Velocity_Control_P.model_param[16];

  // '<S398>:1:42' t14 = phi+q0;
  // '<S398>:1:43' t15 = q0+q1;
  // '<S398>:1:44' t16 = q2+q3;
  localB->t16 = *rtd_RED_Rz_Elbow + *rtd_RED_Rz_Wrist;

  // '<S398>:1:45' t17 = a1.^2;
  localB->t17 = Parker_Velocity_Control_P.model_param[8] *
    Parker_Velocity_Control_P.model_param[8];

  // '<S398>:1:46' t18 = a2.^2;
  // '<S398>:1:47' t19 = a3.^2;
  // '<S398>:1:48' t20 = b0.^2;
  // '<S398>:1:49' t21 = b1.^2;
  localB->t21 = Parker_Velocity_Control_P.model_param[9] *
    Parker_Velocity_Control_P.model_param[9];

  // '<S398>:1:50' t22 = b2.^2;
  // '<S398>:1:51' t48 = -phi;
  // '<S398>:1:52' t49 = -q1;
  // '<S398>:1:53' t50 = m0+m1+m2+m3;
  // '<S398>:1:54' t76 = a1.*b1.*m2.*2.0;
  // '<S398>:1:55' t77 = a1.*b1.*m3.*2.0;
  // '<S398>:1:56' t23 = a1.*t7;
  localB->t23 = Parker_Velocity_Control_P.model_param[8] * localB->t7;

  // '<S398>:1:57' t24 = a1.*t8;
  localB->t24 = Parker_Velocity_Control_P.model_param[8] * localB->t8;

  // '<S398>:1:58' t25 = a1.*t9;
  localB->t25 = Parker_Velocity_Control_P.model_param[8] * localB->t9;

  // '<S398>:1:59' t26 = a3.*t8;
  localB->t26 = Parker_Velocity_Control_P.model_param[12] * localB->t8;

  // '<S398>:1:60' t27 = b0.*t6;
  // '<S398>:1:61' t28 = a1.*t11;
  // '<S398>:1:62' t29 = a1.*t12;
  // '<S398>:1:63' t30 = b0.*t7;
  localB->t30_c = Parker_Velocity_Control_P.model_param[7] * localB->t7;

  // '<S398>:1:64' t31 = b0.*t8;
  localB->t31 = Parker_Velocity_Control_P.model_param[7] * localB->t8;

  // '<S398>:1:65' t32 = b1.*t7;
  localB->t32 = Parker_Velocity_Control_P.model_param[9] * localB->t7;

  // '<S398>:1:66' t33 = a1.*t13;
  localB->t33 = Parker_Velocity_Control_P.model_param[8] * localB->t13;

  // '<S398>:1:67' t34 = b1.*t8;
  localB->t34 = Parker_Velocity_Control_P.model_param[9] * localB->t8;

  // '<S398>:1:68' t35 = b0.*t9;
  localB->t35 = Parker_Velocity_Control_P.model_param[7] * localB->t9;

  // '<S398>:1:69' t36 = b1.*t9;
  localB->t36 = Parker_Velocity_Control_P.model_param[9] * localB->t9;

  // '<S398>:1:70' t37 = b2.*t9;
  localB->t37 = Parker_Velocity_Control_P.model_param[11] * localB->t9;

  // '<S398>:1:71' t38 = b1.*t11;
  // '<S398>:1:72' t39 = b1.*t12;
  // '<S398>:1:73' t40 = b2.*t12;
  localB->t40 = Parker_Velocity_Control_P.model_param[11] * localB->t12;

  // '<S398>:1:74' t41 = b1.*t13;
  localB->t41 = Parker_Velocity_Control_P.model_param[9] * localB->t13;

  // '<S398>:1:75' t42 = cos(t14);
  // '<S398>:1:76' t43 = cos(t15);
  // '<S398>:1:77' t44 = cos(t16);
  localB->t44 = cos(localB->t16);

  // '<S398>:1:78' t45 = q2+t15;
  // '<S398>:1:79' t46 = sin(t14);
  // '<S398>:1:80' t47 = sin(t15);
  // '<S398>:1:81' t52 = t15+t16;
  // '<S398>:1:82' t54 = -t6;
  // '<S398>:1:83' t55 = -t7;
  // '<S398>:1:84' t56 = -t8;
  // '<S398>:1:85' t57 = -t9;
  // '<S398>:1:86' t58 = -t10;
  // '<S398>:1:87' t59 = -t11;
  // '<S398>:1:88' t60 = -t12;
  // '<S398>:1:89' t61 = -t13;
  // '<S398>:1:90' t62 = a1.*t6;
  // '<S398>:1:91' t63 = m2.*t17;
  // '<S398>:1:92' t64 = m3.*t17;
  // '<S398>:1:93' t65 = a2.*t7;
  // '<S398>:1:94' t66 = a2.*t8;
  // '<S398>:1:95' t67 = a3.*t9;
  // '<S398>:1:96' t69 = m2.*t21;
  // '<S398>:1:97' t70 = m3.*t21;
  // '<S398>:1:98' t71 = b2.*t13;
  // '<S398>:1:99' t81 = b2.*t8.*2.0;
  // '<S398>:1:100' t85 = m2.*t4;
  // '<S398>:1:101' t86 = m3.*t4;
  // '<S398>:1:102' t87 = m3.*t5;
  // '<S398>:1:103' t94 = phi+t49;
  // '<S398>:1:104' t95 = q1+q2+t48;
  // '<S398>:1:105' t101 = q1+t16+t48;
  // '<S398>:1:106' t106 = t10+t11+t12;
  // '<S398>:1:107' t51 = cos(t45);
  // '<S398>:1:108' t53 = sin(t45);
  // '<S398>:1:109' t68 = t67;
  // '<S398>:1:110' t72 = t23.*2.0;
  // '<S398>:1:111' t73 = t24.*2.0;
  // '<S398>:1:112' t74 = t25.*2.0;
  // '<S398>:1:113' t75 = t26.*2.0;
  // '<S398>:1:114' t78 = t32.*2.0;
  // '<S398>:1:115' t79 = t33.*2.0;
  // '<S398>:1:116' t80 = t34.*2.0;
  // '<S398>:1:117' t82 = t36.*2.0;
  // '<S398>:1:118' t83 = t37.*2.0;
  // '<S398>:1:119' t84 = t41.*2.0;
  // '<S398>:1:120' t88 = cos(t52);
  // '<S398>:1:121' t89 = -t30;
  // '<S398>:1:122' t90 = -t31;
  // '<S398>:1:123' t91 = -t35;
  // '<S398>:1:124' t92 = -t40;
  // '<S398>:1:125' t93 = sin(t52);
  // '<S398>:1:126' t96 = sin(t94);
  // '<S398>:1:127' t98 = -t85;
  // '<S398>:1:128' t99 = -t86;
  // '<S398>:1:129' t100 = -t87;
  // '<S398>:1:130' t102 = sin(t95);
  localB->t45 = sin((*rtd_RED_Rz_Shoulder + *rtd_RED_Rz_Elbow) +
                    -Parker_Velocity_Control_P.model_param[6]);

  // '<S398>:1:131' t103 = t25+t36;
  // '<S398>:1:132' t104 = t26+t37;
  // '<S398>:1:133' t105 = sin(t101);
  localB->t16 = sin((*rtd_RED_Rz_Shoulder + localB->t16) +
                    -Parker_Velocity_Control_P.model_param[6]);

  // '<S398>:1:134' t115 = t42.*t106;
  // '<S398>:1:135' t118 = t55+t56+t61;
  // '<S398>:1:136' t119 = t58+t59+t60;
  // '<S398>:1:137' t125 = t27+t28+t29+t38+t39;
  // '<S398>:1:138' t126 = -t46.*t106;
  // '<S398>:1:139' t129 = -t43.*(t6+t85+t86);
  // '<S398>:1:140' t130 = -t47.*(t6+t85+t86);
  // '<S398>:1:141' t131 = t23+t24+t32+t33+t34+t41;
  // '<S398>:1:142' t97 = I3+t68;
  localB->t4 = Parker_Velocity_Control_P.model_param[12] * localB->t9 +
    Parker_Velocity_Control_P.model_param[19];

  // '<S398>:1:143' t107 = t74+t82;
  // '<S398>:1:144' t108 = t75+t83;
  // '<S398>:1:145' t109 = t57.*t88;
  // '<S398>:1:146' t110 = t57.*t93;
  // '<S398>:1:147' t111 = t3.*t104;
  // '<S398>:1:148' t112 = t55+t100;
  // '<S398>:1:149' t113 = t44.*t103;
  localB->t9 = (localB->t25 + localB->t36) * localB->t44;

  // '<S398>:1:150' t117 = t91.*t105;
  localB->t117 = -localB->t35 * localB->t16;

  // '<S398>:1:151' t120 = -t53.*(t7+t87);
  // '<S398>:1:152' t121 = t89+t90+t92;
  // '<S398>:1:153' t122 = -t51.*(t7+t87);
  // '<S398>:1:154' t124 = t54+t98+t99;
  // '<S398>:1:155' t127 = -t51.*(t7+t8+t13);
  // '<S398>:1:156' t128 = -t53.*(t7+t8+t13);
  // '<S398>:1:157' t133 = t2.*t131;
  // '<S398>:1:158' t134 = -t102.*(t30+t31+t40);
  localB->t134 = ((localB->t30_c + localB->t31) + localB->t40) * -localB->t45;

  // '<S398>:1:159' t135 = t96.*t125;
  localB->t52 = ((((Parker_Velocity_Control_P.model_param[7] * localB->t6 +
                    Parker_Velocity_Control_P.model_param[8] * localB->t11) +
                   Parker_Velocity_Control_P.model_param[8] * localB->t12) +
                  Parker_Velocity_Control_P.model_param[9] * localB->t11) +
                 Parker_Velocity_Control_P.model_param[9] * localB->t12) * sin
    (Parker_Velocity_Control_P.model_param[6] + -*rtd_RED_Rz_Shoulder);

  // '<S398>:1:160' t136 = t72+t73+t78+t79+t80+t84;
  // '<S398>:1:161' t114 = t3.*t108;
  t114 = (localB->t26 * 2.0 + localB->t37 * 2.0) * localB->t3;

  // '<S398>:1:162' t116 = t44.*t107;
  localB->t25 = (localB->t25 * 2.0 + localB->t36 * 2.0) * localB->t44;

  // '<S398>:1:163' t123 = t97+t111;
  localB->t3 = (localB->t26 + localB->t37) * localB->t3 + localB->t4;

  // '<S398>:1:164' t132 = I2+t65+t66+t71+t81+t97;
  localB->t26 = ((((Parker_Velocity_Control_P.model_param[10] * localB->t7 +
                    Parker_Velocity_Control_P.model_param[18]) +
                   Parker_Velocity_Control_P.model_param[10] * localB->t8) +
                  Parker_Velocity_Control_P.model_param[11] * localB->t13) +
                 Parker_Velocity_Control_P.model_param[11] * localB->t8 * 2.0) +
    localB->t4;

  // '<S398>:1:165' t138 = t2.*t136;
  localB->t37 = (((((localB->t23 * 2.0 + localB->t24 * 2.0) + localB->t32 * 2.0)
                   + localB->t33 * 2.0) + localB->t34 * 2.0) + localB->t41 * 2.0)
    * localB->t2;

  // '<S398>:1:166' t139 = t109+t127;
  // '<S398>:1:167' t140 = t110+t128;
  // '<S398>:1:168' t142 = t110+t120+t130;
  // '<S398>:1:169' t143 = t109+t122+t129;
  // '<S398>:1:170' t137 = t113+t123;
  // '<S398>:1:171' t144 = I1+t62+t63+t64+t69+t70+t76+t77+t132;
  localB->t7 = Parker_Velocity_Control_P.model_param[8] *
    Parker_Velocity_Control_P.model_param[9];
  localB->t6 = (((((((Parker_Velocity_Control_P.model_param[8] * localB->t6 +
                      Parker_Velocity_Control_P.model_param[17]) +
                     Parker_Velocity_Control_P.model_param[15] * localB->t17) +
                    Parker_Velocity_Control_P.model_param[16] * localB->t17) +
                   Parker_Velocity_Control_P.model_param[15] * localB->t21) +
                  Parker_Velocity_Control_P.model_param[16] * localB->t21) +
                 localB->t7 * Parker_Velocity_Control_P.model_param[15] * 2.0) +
                localB->t7 * Parker_Velocity_Control_P.model_param[16] * 2.0) +
    localB->t26;

  // '<S398>:1:172' t145 = t115+t142;
  // '<S398>:1:173' t146 = t126+t143;
  // '<S398>:1:174' t147 = t113+t114+t132+t133;
  localB->t2 = (((((localB->t23 + localB->t24) + localB->t32) + localB->t33) +
                 localB->t34) + localB->t41) * localB->t2 + ((localB->t9 + t114)
    + localB->t26);

  // '<S398>:1:175' t141 = t117+t137;
  // '<S398>:1:176' t148 = t117+t134+t147;
  // '<S398>:1:177' t149 = t114+t116+t117+t134+t135+t138+t144;
  // '<S398>:1:178' InertiaS = reshape([t50,0.0,t146,t143,t139,t109,0.0,t50,t145,t142,t140,t110,t146,t145,I0+t114+t116+t135.*2.0+t138+t144+b0.*t10+b0.*t11+b0.*t12-t35.*t105.*2.0-t102.*(t30.*2.0+t31.*2.0+t40.*2.0),t149,t148,t141,t143,t142,t149,t114+t116+t138+t144,t147,t137,t139,t140,t148,t147,t114+t132,t123,t109,t110,t141,t137,t123,t97],[6,6]); 
  localB->t21 = (((((((((Parker_Velocity_Control_P.model_param[1] + t114) +
                        localB->t25) + localB->t52 * 2.0) + localB->t37) +
                     localB->t6) + Parker_Velocity_Control_P.model_param[7] *
                    Parker_Velocity_Control_P.model_param[14] *
                    Parker_Velocity_Control_P.model_param[7]) +
                   Parker_Velocity_Control_P.model_param[7] * localB->t11) +
                  Parker_Velocity_Control_P.model_param[7] * localB->t12) -
                 localB->t35 * localB->t16 * 2.0) - ((localB->t30_c * 2.0 +
    localB->t31 * 2.0) + localB->t40 * 2.0) * localB->t45;

  // MATLAB Function: '<S396>/MATLAB Function' incorporates:
  //   MATLAB Function: '<S392>/End-Effector Position'
  //   MATLAB Function: '<S396>/Inertia Function '

  // MATLAB Function 'Phase #4:  Return Home/Change RED Behavior/RED+ARM/Transpose Jacobian Controller/Transpose Generalized Jacobian /MATLAB Function': '<S399>:1' 
  // '<S399>:1:5' q0     = q(3);
  // '<S399>:1:6' q1     = q(4);
  // '<S399>:1:7' q2     = q(5);
  // '<S399>:1:8' q3     = q(6);
  // '<S399>:1:10' m0= model_param(1);
  // '<S399>:1:11' I0= model_param(2);
  // '<S399>:1:12' phi= model_param(7);
  // '<S399>:1:13' b0= model_param(8);
  // '<S399>:1:14' a1= model_param(9);
  // '<S399>:1:15' b1= model_param(10);
  // '<S399>:1:16' a2= model_param(11);
  // '<S399>:1:17' b2= model_param(12);
  // '<S399>:1:18' a3= model_param(13);
  // '<S399>:1:19' b3= model_param(14);
  // '<S399>:1:20' m1= model_param(15);
  // '<S399>:1:21' m2= model_param(16);
  // '<S399>:1:22' m3= model_param(17);
  // '<S399>:1:23' I1= model_param(18);
  // '<S399>:1:24' I2= model_param(19);
  // '<S399>:1:25' I3= model_param(20);
  // '<S399>:1:26' l1     = a1+b1;
  // '<S399>:1:27' l2     = a2+b2;
  // '<S399>:1:28' l3     = a3+b3;
  // '<S399>:1:30' w = m0+m1+m2+m3;
  localB->t31 = Parker_Velocity_Control_P.model_param[0] +
    Parker_Velocity_Control_P.model_param[14];
  localB->t11 = localB->t31 + Parker_Velocity_Control_P.model_param[15];
  localB->t35 = localB->t11 + Parker_Velocity_Control_P.model_param[16];

  // '<S399>:1:36' K0     = m0*b0/w;
  localB->t26 = Parker_Velocity_Control_P.model_param[0] *
    Parker_Velocity_Control_P.model_param[7] / localB->t35;

  // '<S399>:1:37' K1     = (m0*l1 + m1*b1)/w;
  localB->t30_c = (Parker_Velocity_Control_P.model_param[0] * l1_tmp +
                   Parker_Velocity_Control_P.model_param[14] *
                   Parker_Velocity_Control_P.model_param[9]) / localB->t35;

  // '<S399>:1:38' K2     = ((m0+m1)*l2 + m2*b2)/w;
  localB->t31 = (localB->t31 * l2_tmp + Parker_Velocity_Control_P.model_param[15]
                 * Parker_Velocity_Control_P.model_param[11]) / localB->t35;

  // '<S399>:1:39' K3     = ((m0+m1+m2)*l3 + m3*b3)/w;
  localB->t35 = (localB->t11 * l3_tmp + Parker_Velocity_Control_P.model_param[16]
                 * Parker_Velocity_Control_P.model_param[13]) / localB->t35;

  // '<S399>:1:42' s0     = sin(phi);
  // '<S399>:1:43' s1     = sin(pi/2+q1);
  localB->t11 = sin(*rtd_RED_Rz_Shoulder + 1.5707963267948966);

  // '<S399>:1:44' s2     = sin(pi/2+q1+q2);
  localB->t23 = (*rtd_RED_Rz_Shoulder + 1.5707963267948966) + *rtd_RED_Rz_Elbow;
  localB->t12 = sin(localB->t23);

  // '<S399>:1:45' s3     = sin(pi/2+q1+q2+q3);
  localB->t24 = localB->t23 + *rtd_RED_Rz_Wrist;
  localB->t16 = sin(localB->t24);

  // '<S399>:1:46' c0     = cos(phi);
  // '<S399>:1:47' c1     = cos(pi/2+q1);
  localB->t17 = cos(*rtd_RED_Rz_Shoulder + 1.5707963267948966);

  // '<S399>:1:48' c2     = cos(pi/2+q1+q2);
  localB->t23 = cos(localB->t23);

  // '<S399>:1:49' c3     = cos(pi/2+q1+q2+q3);
  localB->t24 = cos(localB->t24);

  // '<S399>:1:52' Jm     = [ -K1*s1-K2*s2-K3*s3 -K2*s2-K3*s3 -K3*s3
  // '<S399>:1:53'             K1*c1+K2*c2+K3*c3  K2*c2+K3*c3  K3*c3  ];
  // '<S399>:1:56' Js     = [ -K0*b0*s0-K1*l1*s1-K2*l2*s2-K3*l3*s3
  // '<S399>:1:57'             K0*b0*c0+K1*l1*c1+K2*l2*c2+K3*l3*c3 ];
  // '<S399>:1:60' H      = InertiaS;
  // '<S399>:1:62' Is     = H(3,3);
  // '<S399>:1:63' Im     = [H(3,4), H(3,5), H(3,6)];
  // '<S399>:1:66' J      = Jm - Js*(Is\Im);
  // MATLAB Function 'Phase #4:  Return Home/Change RED Behavior/RED+ARM/Transpose Jacobian Controller/Arm Controller/MATLAB Function': '<S397>:1' 
  // '<S397>:1:5' q1_d   = qdot(4);
  // '<S397>:1:6' q2_d   = qdot(5);
  // '<S397>:1:7' q3_d   = qdot(6);
  // '<S397>:1:10' TAU = Kp*Jt*dx' - Kv*[ q1_d
  // '<S397>:1:11'                        q2_d
  // '<S397>:1:12'                        q3_d];
  // '<S397>:1:14' TAU_out = [ 0
  // '<S397>:1:15'             0
  // '<S397>:1:16'             0
  // '<S397>:1:17'             TAU ];
  localB->t32 = localB->t30_c * l1_tmp;
  localB->t33 = localB->t31 * l2_tmp;
  localB->t34 = localB->t35 * l3_tmp;
  localB->t50_m[0] = ((-localB->t26 * Parker_Velocity_Control_P.model_param[7] *
                       rtb_y_d_tmp - localB->t32 * localB->t11) - localB->t33 *
                      localB->t12) - localB->t34 * localB->t16;
  localB->t50_m[1] = ((localB->t26 * Parker_Velocity_Control_P.model_param[7] *
                       rtb_y_d_tmp_0 + localB->t32 * localB->t17) + localB->t33 *
                      localB->t23) + localB->t34 * localB->t24;
  localB->t6 = ((((((t114 + localB->t25) + localB->t117) + localB->t134) +
                  localB->t52) + localB->t37) + localB->t6) / localB->t21;
  localB->t2 = ((localB->t117 + localB->t134) + localB->t2) / localB->t21;
  localB->t3 = ((localB->t9 + localB->t3) + localB->t117) / localB->t21;
  localB->t9 = localB->t35 * localB->t16;

  // Math: '<S396>/Math Function' incorporates:
  //   MATLAB Function: '<S396>/MATLAB Function'

  localB->t30[0] = (-localB->t30_c * localB->t11 - localB->t31 * localB->t12) -
    localB->t9;
  localB->t30[1] = -localB->t31 * localB->t12 - localB->t9;
  localB->t30[2] = -localB->t35 * localB->t16;

  // MATLAB Function: '<S396>/MATLAB Function'
  localB->t9 = localB->t31 * localB->t23;
  localB->t35 *= localB->t24;

  // Math: '<S396>/Math Function' incorporates:
  //   MATLAB Function: '<S396>/MATLAB Function'

  localB->t30[3] = (localB->t30_c * localB->t17 + localB->t9) + localB->t35;
  localB->t30[4] = localB->t9 + localB->t35;
  localB->t30[5] = localB->t35;
  for (i = 0; i < 2; i++) {
    localB->t50[3 * i] = localB->t50_m[i] * localB->t6;
    localB->t50[3 * i + 1] = localB->t50_m[i] * localB->t2;
    localB->t50[3 * i + 2] = localB->t50_m[i] * localB->t3;
  }

  // MATLAB Function: '<S396>/MATLAB Function'
  for (i = 0; i < 6; i++) {
    localB->TAU_out[i] = localB->t30[i] - localB->t50[i];
  }

  // MATLAB Function: '<S393>/MATLAB Function'
  for (i = 0; i < 2; i++) {
    for (i_0 = 0; i_0 < 3; i_0++) {
      tmp = i_0 + 3 * i;
      localB->t30[tmp] = 0.0;
      localB->t30[tmp] += localB->TAU_out[3 * i] *
        Parker_Velocity_Control_P.Kp[i_0];
      localB->t30[tmp] += localB->TAU_out[3 * i + 1] *
        Parker_Velocity_Control_P.Kp[i_0 + 3];
      localB->t30[tmp] += localB->TAU_out[3 * i + 2] *
        Parker_Velocity_Control_P.Kp[i_0 + 6];
    }
  }

  // Sum: '<S395>/Subtract' incorporates:
  //   Constant: '<S382>/Constant'
  //   Constant: '<S382>/Constant2'
  //   MATLAB Function: '<S392>/End-Effector Position'

  rtb_y_d_tmp_0 = localP->Constant2_Value - (((localB->t17 * l1_tmp +
    Parker_Velocity_Control_P.model_param[7] * rtb_y_d_tmp_0) + localB->t23 *
    l2_tmp) + localB->t24 * l3_tmp);
  l1_tmp = localP->Constant_Value - (((localB->t11 * l1_tmp +
    Parker_Velocity_Control_P.model_param[7] * rtb_y_d_tmp) + localB->t12 *
    l2_tmp) + localB->t16 * l3_tmp);

  // MATLAB Function: '<S393>/MATLAB Function'
  for (i = 0; i < 3; i++) {
    localB->TAU_out[i + 3] = (localB->t30[i + 3] * l1_tmp + localB->t30[i] *
      rtb_y_d_tmp_0) - (Parker_Velocity_Control_P.Kv[i + 6] * *rtd_RED_RzD_Wrist
                        + (Parker_Velocity_Control_P.Kv[i + 3] *
      *rtd_RED_RzD_Elbow + Parker_Velocity_Control_P.Kv[i] *
      *rtd_RED_RzD_Shoulder));
  }

  // Saturate: '<S382>/Saturation'
  if (localB->TAU_out[3] > Parker_Velocity_Control_P.Tz_lim_sharm) {
    *rtd_RED_Tz_Shoulder = Parker_Velocity_Control_P.Tz_lim_sharm;
  } else if (localB->TAU_out[3] < -Parker_Velocity_Control_P.Tz_lim_sharm) {
    *rtd_RED_Tz_Shoulder = -Parker_Velocity_Control_P.Tz_lim_sharm;
  } else {
    *rtd_RED_Tz_Shoulder = localB->TAU_out[3];
  }

  // End of Saturate: '<S382>/Saturation'

  // Saturate: '<S382>/Saturation1'
  if (localB->TAU_out[4] > Parker_Velocity_Control_P.Tz_lim_elarm) {
    *rtd_RED_Tz_Elbow = Parker_Velocity_Control_P.Tz_lim_elarm;
  } else if (localB->TAU_out[4] < -Parker_Velocity_Control_P.Tz_lim_elarm) {
    *rtd_RED_Tz_Elbow = -Parker_Velocity_Control_P.Tz_lim_elarm;
  } else {
    *rtd_RED_Tz_Elbow = localB->TAU_out[4];
  }

  // End of Saturate: '<S382>/Saturation1'

  // Saturate: '<S382>/Saturation3'
  if (localB->TAU_out[5] > Parker_Velocity_Control_P.Tz_lim_wrarm) {
    *rtd_RED_Tz_Wrist = Parker_Velocity_Control_P.Tz_lim_wrarm;
  } else if (localB->TAU_out[5] < -Parker_Velocity_Control_P.Tz_lim_wrarm) {
    *rtd_RED_Tz_Wrist = -Parker_Velocity_Control_P.Tz_lim_wrarm;
  } else {
    *rtd_RED_Tz_Wrist = localB->TAU_out[5];
  }

  // End of Saturate: '<S382>/Saturation3'
}

static real_T Park_rt_urand_Upu32_Yd_f_pw_snf(uint32_T *u)
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

  Parker_Velocity_Control_B.lo = *u % 127773U * 16807U;
  Parker_Velocity_Control_B.hi = *u / 127773U * 2836U;
  if (Parker_Velocity_Control_B.lo < Parker_Velocity_Control_B.hi) {
    *u = 2147483647U - (Parker_Velocity_Control_B.hi -
                        Parker_Velocity_Control_B.lo);
  } else {
    *u = Parker_Velocity_Control_B.lo - Parker_Velocity_Control_B.hi;
  }

  return static_cast<real_T>(*u) * 4.6566128752457969E-10;
}

static real_T Park_rt_nrand_Upu32_Yd_f_pw_snf(uint32_T *u)
{
  real_T y;

  // Normal (Gaussian) random number generator
  do {
    Parker_Velocity_Control_B.sr = 2.0 * Park_rt_urand_Upu32_Yd_f_pw_snf(u) -
      1.0;
    Parker_Velocity_Control_B.si = 2.0 * Park_rt_urand_Upu32_Yd_f_pw_snf(u) -
      1.0;
    Parker_Velocity_Control_B.si = Parker_Velocity_Control_B.sr *
      Parker_Velocity_Control_B.sr + Parker_Velocity_Control_B.si *
      Parker_Velocity_Control_B.si;
  } while (Parker_Velocity_Control_B.si > 1.0);

  y = sqrt(-2.0 * log(Parker_Velocity_Control_B.si) /
           Parker_Velocity_Control_B.si) * Parker_Velocity_Control_B.sr;
  return y;
}

// Function for MATLAB Function: '<S490>/Dynamics Model'
static real_T Parker_Velocity_Control_xnrm2(int32_T n, const real_T x[36],
  int32_T ix0)
{
  real_T y;
  y = 0.0;
  Parker_Velocity_Control_B.scale_dy = 3.3121686421112381E-170;
  Parker_Velocity_Control_B.kend_b = (ix0 + n) - 1;
  Parker_Velocity_Control_B.k_l = ix0;
  while (Parker_Velocity_Control_B.k_l <= Parker_Velocity_Control_B.kend_b) {
    Parker_Velocity_Control_B.absxk_l = fabs(x[Parker_Velocity_Control_B.k_l - 1]);
    if (Parker_Velocity_Control_B.absxk_l > Parker_Velocity_Control_B.scale_dy)
    {
      Parker_Velocity_Control_B.t_o = Parker_Velocity_Control_B.scale_dy /
        Parker_Velocity_Control_B.absxk_l;
      y = y * Parker_Velocity_Control_B.t_o * Parker_Velocity_Control_B.t_o +
        1.0;
      Parker_Velocity_Control_B.scale_dy = Parker_Velocity_Control_B.absxk_l;
    } else {
      Parker_Velocity_Control_B.t_o = Parker_Velocity_Control_B.absxk_l /
        Parker_Velocity_Control_B.scale_dy;
      y += Parker_Velocity_Control_B.t_o * Parker_Velocity_Control_B.t_o;
    }

    Parker_Velocity_Control_B.k_l++;
  }

  return Parker_Velocity_Control_B.scale_dy * sqrt(y);
}

// Function for MATLAB Function: '<S490>/Dynamics Model'
static real_T Parker_Velocity_Control_xnrm2_l(int32_T n, const real_T x[6],
  int32_T ix0)
{
  real_T y;
  y = 0.0;
  Parker_Velocity_Control_B.scale_d = 3.3121686421112381E-170;
  Parker_Velocity_Control_B.kend = (ix0 + n) - 1;
  Parker_Velocity_Control_B.k = ix0;
  while (Parker_Velocity_Control_B.k <= Parker_Velocity_Control_B.kend) {
    Parker_Velocity_Control_B.absxk = fabs(x[Parker_Velocity_Control_B.k - 1]);
    if (Parker_Velocity_Control_B.absxk > Parker_Velocity_Control_B.scale_d) {
      Parker_Velocity_Control_B.t = Parker_Velocity_Control_B.scale_d /
        Parker_Velocity_Control_B.absxk;
      y = y * Parker_Velocity_Control_B.t * Parker_Velocity_Control_B.t + 1.0;
      Parker_Velocity_Control_B.scale_d = Parker_Velocity_Control_B.absxk;
    } else {
      Parker_Velocity_Control_B.t = Parker_Velocity_Control_B.absxk /
        Parker_Velocity_Control_B.scale_d;
      y += Parker_Velocity_Control_B.t * Parker_Velocity_Control_B.t;
    }

    Parker_Velocity_Control_B.k++;
  }

  return Parker_Velocity_Control_B.scale_d * sqrt(y);
}

// Function for MATLAB Function: '<S490>/Dynamics Model'
static void Parker_Velocity_Contro_xaxpy_ot(int32_T n, real_T a, const real_T x
  [6], int32_T ix0, real_T y[36], int32_T iy0)
{
  if (!(a == 0.0)) {
    Parker_Velocity_Control_B.ix_oy = ix0 - 1;
    Parker_Velocity_Control_B.iy_nv = iy0 - 1;
    Parker_Velocity_Control_B.k_m = 0;
    while (Parker_Velocity_Control_B.k_m <= n - 1) {
      y[Parker_Velocity_Control_B.iy_nv] += a *
        x[Parker_Velocity_Control_B.ix_oy];
      Parker_Velocity_Control_B.ix_oy++;
      Parker_Velocity_Control_B.iy_nv++;
      Parker_Velocity_Control_B.k_m++;
    }
  }
}

// Function for MATLAB Function: '<S490>/Dynamics Model'
static void Parker_Velocity_Control_xaxpy_o(int32_T n, real_T a, const real_T x
  [36], int32_T ix0, real_T y[6], int32_T iy0)
{
  if (!(a == 0.0)) {
    Parker_Velocity_Control_B.ix_o = ix0 - 1;
    Parker_Velocity_Control_B.iy_n = iy0 - 1;
    Parker_Velocity_Control_B.k_i = 0;
    while (Parker_Velocity_Control_B.k_i <= n - 1) {
      y[Parker_Velocity_Control_B.iy_n] += a * x[Parker_Velocity_Control_B.ix_o];
      Parker_Velocity_Control_B.ix_o++;
      Parker_Velocity_Control_B.iy_n++;
      Parker_Velocity_Control_B.k_i++;
    }
  }
}

// Function for MATLAB Function: '<S490>/Dynamics Model'
static real_T Parker_Velocity_Control_xdotc(int32_T n, const real_T x[36],
  int32_T ix0, const real_T y[36], int32_T iy0)
{
  real_T d;
  d = 0.0;
  Parker_Velocity_Control_B.ix_a = ix0;
  Parker_Velocity_Control_B.iy_ju = iy0;
  Parker_Velocity_Control_B.k_j = 0;
  while (Parker_Velocity_Control_B.k_j <= n - 1) {
    d += x[Parker_Velocity_Control_B.ix_a - 1] *
      y[Parker_Velocity_Control_B.iy_ju - 1];
    Parker_Velocity_Control_B.ix_a++;
    Parker_Velocity_Control_B.iy_ju++;
    Parker_Velocity_Control_B.k_j++;
  }

  return d;
}

// Function for MATLAB Function: '<S490>/Dynamics Model'
static void Parker_Velocity_Control_xaxpy(int32_T n, real_T a, int32_T ix0,
  real_T y[36], int32_T iy0)
{
  if (!(a == 0.0)) {
    Parker_Velocity_Control_B.ix_bj = ix0 - 1;
    Parker_Velocity_Control_B.iy_j = iy0 - 1;
    Parker_Velocity_Control_B.k_f = 0;
    while (Parker_Velocity_Control_B.k_f <= n - 1) {
      y[Parker_Velocity_Control_B.iy_j] += a * y[Parker_Velocity_Control_B.ix_bj];
      Parker_Velocity_Control_B.ix_bj++;
      Parker_Velocity_Control_B.iy_j++;
      Parker_Velocity_Control_B.k_f++;
    }
  }
}

// Function for MATLAB Function: '<S490>/Dynamics Model'
static void Parker_Velocity_Control_xscal(real_T a, real_T x[36], int32_T ix0)
{
  Parker_Velocity_Control_B.k_c = ix0;
  while (Parker_Velocity_Control_B.k_c <= ix0 + 5) {
    x[Parker_Velocity_Control_B.k_c - 1] *= a;
    Parker_Velocity_Control_B.k_c++;
  }
}

// Function for MATLAB Function: '<S490>/Dynamics Model'
static void Parker_Velocity_Control_xswap(real_T x[36], int32_T ix0, int32_T iy0)
{
  Parker_Velocity_Control_B.ix_b = ix0 - 1;
  Parker_Velocity_Control_B.iy_d = iy0 - 1;
  for (Parker_Velocity_Control_B.k_e = 0; Parker_Velocity_Control_B.k_e < 6;
       Parker_Velocity_Control_B.k_e++) {
    Parker_Velocity_Control_B.temp_b = x[Parker_Velocity_Control_B.ix_b];
    x[Parker_Velocity_Control_B.ix_b] = x[Parker_Velocity_Control_B.iy_d];
    x[Parker_Velocity_Control_B.iy_d] = Parker_Velocity_Control_B.temp_b;
    Parker_Velocity_Control_B.ix_b++;
    Parker_Velocity_Control_B.iy_d++;
  }
}

// Function for MATLAB Function: '<S490>/Dynamics Model'
static void Parker_Velocity_Control_xrotg(real_T *a, real_T *b, real_T *c,
  real_T *s)
{
  Parker_Velocity_Control_B.roe = *b;
  Parker_Velocity_Control_B.absa = fabs(*a);
  Parker_Velocity_Control_B.absb = fabs(*b);
  if (Parker_Velocity_Control_B.absa > Parker_Velocity_Control_B.absb) {
    Parker_Velocity_Control_B.roe = *a;
  }

  Parker_Velocity_Control_B.scale = Parker_Velocity_Control_B.absa +
    Parker_Velocity_Control_B.absb;
  if (Parker_Velocity_Control_B.scale == 0.0) {
    *s = 0.0;
    *c = 1.0;
    *b = 0.0;
  } else {
    Parker_Velocity_Control_B.ads = Parker_Velocity_Control_B.absa /
      Parker_Velocity_Control_B.scale;
    Parker_Velocity_Control_B.bds = Parker_Velocity_Control_B.absb /
      Parker_Velocity_Control_B.scale;
    Parker_Velocity_Control_B.scale *= sqrt(Parker_Velocity_Control_B.ads *
      Parker_Velocity_Control_B.ads + Parker_Velocity_Control_B.bds *
      Parker_Velocity_Control_B.bds);
    if (Parker_Velocity_Control_B.roe < 0.0) {
      Parker_Velocity_Control_B.scale = -Parker_Velocity_Control_B.scale;
    }

    *c = *a / Parker_Velocity_Control_B.scale;
    *s = *b / Parker_Velocity_Control_B.scale;
    if (Parker_Velocity_Control_B.absa > Parker_Velocity_Control_B.absb) {
      *b = *s;
    } else if (*c != 0.0) {
      *b = 1.0 / *c;
    } else {
      *b = 1.0;
    }
  }

  *a = Parker_Velocity_Control_B.scale;
}

// Function for MATLAB Function: '<S490>/Dynamics Model'
static void Parker_Velocity_Control_xrot(real_T x[36], int32_T ix0, int32_T iy0,
  real_T c, real_T s)
{
  Parker_Velocity_Control_B.ix = ix0 - 1;
  Parker_Velocity_Control_B.iy = iy0 - 1;
  for (Parker_Velocity_Control_B.k_h = 0; Parker_Velocity_Control_B.k_h < 6;
       Parker_Velocity_Control_B.k_h++) {
    Parker_Velocity_Control_B.temp = c * x[Parker_Velocity_Control_B.ix] + s *
      x[Parker_Velocity_Control_B.iy];
    x[Parker_Velocity_Control_B.iy] = c * x[Parker_Velocity_Control_B.iy] - s *
      x[Parker_Velocity_Control_B.ix];
    x[Parker_Velocity_Control_B.ix] = Parker_Velocity_Control_B.temp;
    Parker_Velocity_Control_B.iy++;
    Parker_Velocity_Control_B.ix++;
  }
}

// Function for MATLAB Function: '<S490>/Dynamics Model'
static void Parker_Velocity_Control_svd(const real_T A[36], real_T U[36], real_T
  s[6], real_T V[36])
{
  int32_T exitg1;
  boolean_T exitg2;
  memcpy(&Parker_Velocity_Control_B.b_A[0], &A[0], 36U * sizeof(real_T));
  for (Parker_Velocity_Control_B.i_n = 0; Parker_Velocity_Control_B.i_n < 6;
       Parker_Velocity_Control_B.i_n++) {
    Parker_Velocity_Control_B.b_s[Parker_Velocity_Control_B.i_n] = 0.0;
    Parker_Velocity_Control_B.e[Parker_Velocity_Control_B.i_n] = 0.0;
    Parker_Velocity_Control_B.work[Parker_Velocity_Control_B.i_n] = 0.0;
  }

  memset(&U[0], 0, 36U * sizeof(real_T));
  memset(&Parker_Velocity_Control_B.Vf[0], 0, 36U * sizeof(real_T));
  for (Parker_Velocity_Control_B.i_n = 0; Parker_Velocity_Control_B.i_n < 5;
       Parker_Velocity_Control_B.i_n++) {
    Parker_Velocity_Control_B.qq = 6 * Parker_Velocity_Control_B.i_n +
      Parker_Velocity_Control_B.i_n;
    Parker_Velocity_Control_B.apply_transform = false;
    Parker_Velocity_Control_B.nrm = Parker_Velocity_Control_xnrm2(6 -
      Parker_Velocity_Control_B.i_n, Parker_Velocity_Control_B.b_A,
      Parker_Velocity_Control_B.qq + 1);
    if (Parker_Velocity_Control_B.nrm > 0.0) {
      Parker_Velocity_Control_B.apply_transform = true;
      if (Parker_Velocity_Control_B.b_A[Parker_Velocity_Control_B.qq] < 0.0) {
        Parker_Velocity_Control_B.b_s[Parker_Velocity_Control_B.i_n] =
          -Parker_Velocity_Control_B.nrm;
      } else {
        Parker_Velocity_Control_B.b_s[Parker_Velocity_Control_B.i_n] =
          Parker_Velocity_Control_B.nrm;
      }

      if (fabs(Parker_Velocity_Control_B.b_s[Parker_Velocity_Control_B.i_n]) >=
          1.0020841800044864E-292) {
        Parker_Velocity_Control_B.nrm = 1.0 /
          Parker_Velocity_Control_B.b_s[Parker_Velocity_Control_B.i_n];
        Parker_Velocity_Control_B.qp1q = (Parker_Velocity_Control_B.qq -
          Parker_Velocity_Control_B.i_n) + 6;
        Parker_Velocity_Control_B.qjj = Parker_Velocity_Control_B.qq;
        while (Parker_Velocity_Control_B.qjj + 1 <=
               Parker_Velocity_Control_B.qp1q) {
          Parker_Velocity_Control_B.b_A[Parker_Velocity_Control_B.qjj] *=
            Parker_Velocity_Control_B.nrm;
          Parker_Velocity_Control_B.qjj++;
        }
      } else {
        Parker_Velocity_Control_B.qp1q = (Parker_Velocity_Control_B.qq -
          Parker_Velocity_Control_B.i_n) + 6;
        Parker_Velocity_Control_B.qjj = Parker_Velocity_Control_B.qq;
        while (Parker_Velocity_Control_B.qjj + 1 <=
               Parker_Velocity_Control_B.qp1q) {
          Parker_Velocity_Control_B.b_A[Parker_Velocity_Control_B.qjj] /=
            Parker_Velocity_Control_B.b_s[Parker_Velocity_Control_B.i_n];
          Parker_Velocity_Control_B.qjj++;
        }
      }

      Parker_Velocity_Control_B.b_A[Parker_Velocity_Control_B.qq]++;
      Parker_Velocity_Control_B.b_s[Parker_Velocity_Control_B.i_n] =
        -Parker_Velocity_Control_B.b_s[Parker_Velocity_Control_B.i_n];
    } else {
      Parker_Velocity_Control_B.b_s[Parker_Velocity_Control_B.i_n] = 0.0;
    }

    Parker_Velocity_Control_B.qp1q = Parker_Velocity_Control_B.i_n + 1;
    while (Parker_Velocity_Control_B.qp1q + 1 < 7) {
      Parker_Velocity_Control_B.qjj = 6 * Parker_Velocity_Control_B.qp1q +
        Parker_Velocity_Control_B.i_n;
      if (Parker_Velocity_Control_B.apply_transform) {
        Parker_Velocity_Control_xaxpy(6 - Parker_Velocity_Control_B.i_n,
          -(Parker_Velocity_Control_xdotc(6 - Parker_Velocity_Control_B.i_n,
          Parker_Velocity_Control_B.b_A, Parker_Velocity_Control_B.qq + 1,
          Parker_Velocity_Control_B.b_A, Parker_Velocity_Control_B.qjj + 1) /
            Parker_Velocity_Control_B.b_A[Parker_Velocity_Control_B.i_n + 6 *
            Parker_Velocity_Control_B.i_n]), Parker_Velocity_Control_B.qq + 1,
          Parker_Velocity_Control_B.b_A, Parker_Velocity_Control_B.qjj + 1);
      }

      Parker_Velocity_Control_B.e[Parker_Velocity_Control_B.qp1q] =
        Parker_Velocity_Control_B.b_A[Parker_Velocity_Control_B.qjj];
      Parker_Velocity_Control_B.qp1q++;
    }

    Parker_Velocity_Control_B.qq = Parker_Velocity_Control_B.i_n;
    while (Parker_Velocity_Control_B.qq + 1 < 7) {
      U[Parker_Velocity_Control_B.qq + 6 * Parker_Velocity_Control_B.i_n] =
        Parker_Velocity_Control_B.b_A[6 * Parker_Velocity_Control_B.i_n +
        Parker_Velocity_Control_B.qq];
      Parker_Velocity_Control_B.qq++;
    }

    if (Parker_Velocity_Control_B.i_n + 1 <= 4) {
      Parker_Velocity_Control_B.nrm = Parker_Velocity_Control_xnrm2_l(5 -
        Parker_Velocity_Control_B.i_n, Parker_Velocity_Control_B.e,
        Parker_Velocity_Control_B.i_n + 2);
      if (Parker_Velocity_Control_B.nrm == 0.0) {
        Parker_Velocity_Control_B.e[Parker_Velocity_Control_B.i_n] = 0.0;
      } else {
        if (Parker_Velocity_Control_B.e[Parker_Velocity_Control_B.i_n + 1] < 0.0)
        {
          Parker_Velocity_Control_B.e[Parker_Velocity_Control_B.i_n] =
            -Parker_Velocity_Control_B.nrm;
        } else {
          Parker_Velocity_Control_B.e[Parker_Velocity_Control_B.i_n] =
            Parker_Velocity_Control_B.nrm;
        }

        Parker_Velocity_Control_B.nrm =
          Parker_Velocity_Control_B.e[Parker_Velocity_Control_B.i_n];
        if (fabs(Parker_Velocity_Control_B.e[Parker_Velocity_Control_B.i_n]) >=
            1.0020841800044864E-292) {
          Parker_Velocity_Control_B.nrm = 1.0 /
            Parker_Velocity_Control_B.e[Parker_Velocity_Control_B.i_n];
          Parker_Velocity_Control_B.qq = Parker_Velocity_Control_B.i_n + 1;
          while (Parker_Velocity_Control_B.qq + 1 <= 6) {
            Parker_Velocity_Control_B.e[Parker_Velocity_Control_B.qq] *=
              Parker_Velocity_Control_B.nrm;
            Parker_Velocity_Control_B.qq++;
          }
        } else {
          Parker_Velocity_Control_B.qq = Parker_Velocity_Control_B.i_n + 1;
          while (Parker_Velocity_Control_B.qq + 1 <= 6) {
            Parker_Velocity_Control_B.e[Parker_Velocity_Control_B.qq] /=
              Parker_Velocity_Control_B.nrm;
            Parker_Velocity_Control_B.qq++;
          }
        }

        Parker_Velocity_Control_B.e[Parker_Velocity_Control_B.i_n + 1]++;
        Parker_Velocity_Control_B.e[Parker_Velocity_Control_B.i_n] =
          -Parker_Velocity_Control_B.e[Parker_Velocity_Control_B.i_n];
        Parker_Velocity_Control_B.qq = Parker_Velocity_Control_B.i_n + 1;
        while (Parker_Velocity_Control_B.qq + 1 < 7) {
          Parker_Velocity_Control_B.work[Parker_Velocity_Control_B.qq] = 0.0;
          Parker_Velocity_Control_B.qq++;
        }

        Parker_Velocity_Control_B.qq = Parker_Velocity_Control_B.i_n + 1;
        while (Parker_Velocity_Control_B.qq + 1 < 7) {
          Parker_Velocity_Control_xaxpy_o(5 - Parker_Velocity_Control_B.i_n,
            Parker_Velocity_Control_B.e[Parker_Velocity_Control_B.qq],
            Parker_Velocity_Control_B.b_A, (Parker_Velocity_Control_B.i_n + 6 *
            Parker_Velocity_Control_B.qq) + 2, Parker_Velocity_Control_B.work,
            Parker_Velocity_Control_B.i_n + 2);
          Parker_Velocity_Control_B.qq++;
        }

        Parker_Velocity_Control_B.qq = Parker_Velocity_Control_B.i_n + 1;
        while (Parker_Velocity_Control_B.qq + 1 < 7) {
          Parker_Velocity_Contro_xaxpy_ot(5 - Parker_Velocity_Control_B.i_n,
            -Parker_Velocity_Control_B.e[Parker_Velocity_Control_B.qq] /
            Parker_Velocity_Control_B.e[Parker_Velocity_Control_B.i_n + 1],
            Parker_Velocity_Control_B.work, Parker_Velocity_Control_B.i_n + 2,
            Parker_Velocity_Control_B.b_A, (Parker_Velocity_Control_B.i_n + 6 *
            Parker_Velocity_Control_B.qq) + 2);
          Parker_Velocity_Control_B.qq++;
        }
      }

      Parker_Velocity_Control_B.qq = Parker_Velocity_Control_B.i_n + 1;
      while (Parker_Velocity_Control_B.qq + 1 < 7) {
        Parker_Velocity_Control_B.Vf[Parker_Velocity_Control_B.qq + 6 *
          Parker_Velocity_Control_B.i_n] =
          Parker_Velocity_Control_B.e[Parker_Velocity_Control_B.qq];
        Parker_Velocity_Control_B.qq++;
      }
    }
  }

  Parker_Velocity_Control_B.i_n = 4;
  Parker_Velocity_Control_B.b_s[5] = Parker_Velocity_Control_B.b_A[35];
  Parker_Velocity_Control_B.e[4] = Parker_Velocity_Control_B.b_A[34];
  Parker_Velocity_Control_B.e[5] = 0.0;
  for (Parker_Velocity_Control_B.qq = 0; Parker_Velocity_Control_B.qq < 6;
       Parker_Velocity_Control_B.qq++) {
    U[Parker_Velocity_Control_B.qq + 30] = 0.0;
  }

  U[35] = 1.0;
  for (Parker_Velocity_Control_B.qp1q = 4; Parker_Velocity_Control_B.qp1q >= 0;
       Parker_Velocity_Control_B.qp1q--) {
    Parker_Velocity_Control_B.qq = 6 * Parker_Velocity_Control_B.qp1q +
      Parker_Velocity_Control_B.qp1q;
    if (Parker_Velocity_Control_B.b_s[Parker_Velocity_Control_B.qp1q] != 0.0) {
      Parker_Velocity_Control_B.qp1jj = Parker_Velocity_Control_B.qp1q + 1;
      while (Parker_Velocity_Control_B.qp1jj + 1 < 7) {
        Parker_Velocity_Control_B.qjj = (6 * Parker_Velocity_Control_B.qp1jj +
          Parker_Velocity_Control_B.qp1q) + 1;
        Parker_Velocity_Control_xaxpy(6 - Parker_Velocity_Control_B.qp1q,
          -(Parker_Velocity_Control_xdotc(6 - Parker_Velocity_Control_B.qp1q, U,
          Parker_Velocity_Control_B.qq + 1, U, Parker_Velocity_Control_B.qjj) /
            U[Parker_Velocity_Control_B.qq]), Parker_Velocity_Control_B.qq + 1,
          U, Parker_Velocity_Control_B.qjj);
        Parker_Velocity_Control_B.qp1jj++;
      }

      Parker_Velocity_Control_B.qjj = Parker_Velocity_Control_B.qp1q;
      while (Parker_Velocity_Control_B.qjj + 1 < 7) {
        Parker_Velocity_Control_B.qp1jj = 6 * Parker_Velocity_Control_B.qp1q +
          Parker_Velocity_Control_B.qjj;
        U[Parker_Velocity_Control_B.qp1jj] = -U[Parker_Velocity_Control_B.qp1jj];
        Parker_Velocity_Control_B.qjj++;
      }

      U[Parker_Velocity_Control_B.qq]++;
      Parker_Velocity_Control_B.qq = 0;
      while (Parker_Velocity_Control_B.qq <= Parker_Velocity_Control_B.qp1q - 1)
      {
        U[Parker_Velocity_Control_B.qq + 6 * Parker_Velocity_Control_B.qp1q] =
          0.0;
        Parker_Velocity_Control_B.qq++;
      }
    } else {
      for (Parker_Velocity_Control_B.qjj = 0; Parker_Velocity_Control_B.qjj < 6;
           Parker_Velocity_Control_B.qjj++) {
        U[Parker_Velocity_Control_B.qjj + 6 * Parker_Velocity_Control_B.qp1q] =
          0.0;
      }

      U[Parker_Velocity_Control_B.qq] = 1.0;
    }
  }

  for (Parker_Velocity_Control_B.qq = 5; Parker_Velocity_Control_B.qq >= 0;
       Parker_Velocity_Control_B.qq--) {
    if ((Parker_Velocity_Control_B.qq + 1 <= 4) &&
        (Parker_Velocity_Control_B.e[Parker_Velocity_Control_B.qq] != 0.0)) {
      Parker_Velocity_Control_B.qp1q = (6 * Parker_Velocity_Control_B.qq +
        Parker_Velocity_Control_B.qq) + 2;
      Parker_Velocity_Control_B.qjj = Parker_Velocity_Control_B.qq + 1;
      while (Parker_Velocity_Control_B.qjj + 1 < 7) {
        Parker_Velocity_Control_B.qp1jj = (6 * Parker_Velocity_Control_B.qjj +
          Parker_Velocity_Control_B.qq) + 2;
        Parker_Velocity_Control_xaxpy(5 - Parker_Velocity_Control_B.qq,
          -(Parker_Velocity_Control_xdotc(5 - Parker_Velocity_Control_B.qq,
          Parker_Velocity_Control_B.Vf, Parker_Velocity_Control_B.qp1q,
          Parker_Velocity_Control_B.Vf, Parker_Velocity_Control_B.qp1jj) /
            Parker_Velocity_Control_B.Vf[Parker_Velocity_Control_B.qp1q - 1]),
          Parker_Velocity_Control_B.qp1q, Parker_Velocity_Control_B.Vf,
          Parker_Velocity_Control_B.qp1jj);
        Parker_Velocity_Control_B.qjj++;
      }
    }

    for (Parker_Velocity_Control_B.qp1q = 0; Parker_Velocity_Control_B.qp1q < 6;
         Parker_Velocity_Control_B.qp1q++) {
      Parker_Velocity_Control_B.Vf[Parker_Velocity_Control_B.qp1q + 6 *
        Parker_Velocity_Control_B.qq] = 0.0;
    }

    Parker_Velocity_Control_B.Vf[Parker_Velocity_Control_B.qq + 6 *
      Parker_Velocity_Control_B.qq] = 1.0;
  }

  for (Parker_Velocity_Control_B.qq = 0; Parker_Velocity_Control_B.qq < 6;
       Parker_Velocity_Control_B.qq++) {
    Parker_Velocity_Control_B.ztest =
      Parker_Velocity_Control_B.e[Parker_Velocity_Control_B.qq];
    if (Parker_Velocity_Control_B.b_s[Parker_Velocity_Control_B.qq] != 0.0) {
      Parker_Velocity_Control_B.rt = fabs
        (Parker_Velocity_Control_B.b_s[Parker_Velocity_Control_B.qq]);
      Parker_Velocity_Control_B.nrm =
        Parker_Velocity_Control_B.b_s[Parker_Velocity_Control_B.qq] /
        Parker_Velocity_Control_B.rt;
      Parker_Velocity_Control_B.b_s[Parker_Velocity_Control_B.qq] =
        Parker_Velocity_Control_B.rt;
      if (Parker_Velocity_Control_B.qq + 1 < 6) {
        Parker_Velocity_Control_B.ztest =
          Parker_Velocity_Control_B.e[Parker_Velocity_Control_B.qq] /
          Parker_Velocity_Control_B.nrm;
      }

      Parker_Velocity_Control_xscal(Parker_Velocity_Control_B.nrm, U, 6 *
        Parker_Velocity_Control_B.qq + 1);
    }

    if ((Parker_Velocity_Control_B.qq + 1 < 6) &&
        (Parker_Velocity_Control_B.ztest != 0.0)) {
      Parker_Velocity_Control_B.rt = fabs(Parker_Velocity_Control_B.ztest);
      Parker_Velocity_Control_B.nrm = Parker_Velocity_Control_B.rt /
        Parker_Velocity_Control_B.ztest;
      Parker_Velocity_Control_B.ztest = Parker_Velocity_Control_B.rt;
      Parker_Velocity_Control_B.b_s[Parker_Velocity_Control_B.qq + 1] *=
        Parker_Velocity_Control_B.nrm;
      Parker_Velocity_Control_xscal(Parker_Velocity_Control_B.nrm,
        Parker_Velocity_Control_B.Vf, 6 * (Parker_Velocity_Control_B.qq + 1) + 1);
    }

    Parker_Velocity_Control_B.e[Parker_Velocity_Control_B.qq] =
      Parker_Velocity_Control_B.ztest;
  }

  Parker_Velocity_Control_B.qq = 0;
  Parker_Velocity_Control_B.nrm = 0.0;
  for (Parker_Velocity_Control_B.qp1q = 0; Parker_Velocity_Control_B.qp1q < 6;
       Parker_Velocity_Control_B.qp1q++) {
    Parker_Velocity_Control_B.ztest = fabs
      (Parker_Velocity_Control_B.b_s[Parker_Velocity_Control_B.qp1q]);
    Parker_Velocity_Control_B.rt = fabs
      (Parker_Velocity_Control_B.e[Parker_Velocity_Control_B.qp1q]);
    if ((Parker_Velocity_Control_B.ztest > Parker_Velocity_Control_B.rt) ||
        rtIsNaN(Parker_Velocity_Control_B.rt)) {
      Parker_Velocity_Control_B.rt = Parker_Velocity_Control_B.ztest;
    }

    if ((!(Parker_Velocity_Control_B.nrm > Parker_Velocity_Control_B.rt)) &&
        (!rtIsNaN(Parker_Velocity_Control_B.rt))) {
      Parker_Velocity_Control_B.nrm = Parker_Velocity_Control_B.rt;
    }
  }

  while ((Parker_Velocity_Control_B.i_n + 2 > 0) &&
         (Parker_Velocity_Control_B.qq < 75)) {
    Parker_Velocity_Control_B.qp1jj = Parker_Velocity_Control_B.i_n + 1;
    do {
      exitg1 = 0;
      Parker_Velocity_Control_B.qp1q = Parker_Velocity_Control_B.qp1jj;
      if (Parker_Velocity_Control_B.qp1jj == 0) {
        exitg1 = 1;
      } else {
        Parker_Velocity_Control_B.rt = fabs
          (Parker_Velocity_Control_B.e[Parker_Velocity_Control_B.qp1jj - 1]);
        if (Parker_Velocity_Control_B.rt <= (fabs
             (Parker_Velocity_Control_B.b_s[Parker_Velocity_Control_B.qp1jj - 1])
             + fabs
             (Parker_Velocity_Control_B.b_s[Parker_Velocity_Control_B.qp1jj])) *
            2.2204460492503131E-16) {
          Parker_Velocity_Control_B.e[Parker_Velocity_Control_B.qp1jj - 1] = 0.0;
          exitg1 = 1;
        } else if ((Parker_Velocity_Control_B.rt <= 1.0020841800044864E-292) ||
                   ((Parker_Velocity_Control_B.qq > 20) &&
                    (Parker_Velocity_Control_B.rt <= 2.2204460492503131E-16 *
                     Parker_Velocity_Control_B.nrm))) {
          Parker_Velocity_Control_B.e[Parker_Velocity_Control_B.qp1jj - 1] = 0.0;
          exitg1 = 1;
        } else {
          Parker_Velocity_Control_B.qp1jj--;
        }
      }
    } while (exitg1 == 0);

    if (Parker_Velocity_Control_B.i_n + 1 == Parker_Velocity_Control_B.qp1jj) {
      Parker_Velocity_Control_B.qp1jj = 4;
    } else {
      Parker_Velocity_Control_B.qjj = Parker_Velocity_Control_B.i_n + 2;
      Parker_Velocity_Control_B.k_ii = Parker_Velocity_Control_B.i_n + 2;
      exitg2 = false;
      while ((!exitg2) && (Parker_Velocity_Control_B.k_ii >=
                           Parker_Velocity_Control_B.qp1jj)) {
        Parker_Velocity_Control_B.qjj = Parker_Velocity_Control_B.k_ii;
        if (Parker_Velocity_Control_B.k_ii == Parker_Velocity_Control_B.qp1jj) {
          exitg2 = true;
        } else {
          Parker_Velocity_Control_B.rt = 0.0;
          if (Parker_Velocity_Control_B.k_ii < Parker_Velocity_Control_B.i_n + 2)
          {
            Parker_Velocity_Control_B.rt = fabs
              (Parker_Velocity_Control_B.e[Parker_Velocity_Control_B.k_ii - 1]);
          }

          if (Parker_Velocity_Control_B.k_ii > Parker_Velocity_Control_B.qp1jj +
              1) {
            Parker_Velocity_Control_B.rt += fabs
              (Parker_Velocity_Control_B.e[Parker_Velocity_Control_B.k_ii - 2]);
          }

          Parker_Velocity_Control_B.ztest = fabs
            (Parker_Velocity_Control_B.b_s[Parker_Velocity_Control_B.k_ii - 1]);
          if ((Parker_Velocity_Control_B.ztest <= 2.2204460492503131E-16 *
               Parker_Velocity_Control_B.rt) || (Parker_Velocity_Control_B.ztest
               <= 1.0020841800044864E-292)) {
            Parker_Velocity_Control_B.b_s[Parker_Velocity_Control_B.k_ii - 1] =
              0.0;
            exitg2 = true;
          } else {
            Parker_Velocity_Control_B.k_ii--;
          }
        }
      }

      if (Parker_Velocity_Control_B.qjj == Parker_Velocity_Control_B.qp1jj) {
        Parker_Velocity_Control_B.qp1jj = 3;
      } else if (Parker_Velocity_Control_B.i_n + 2 ==
                 Parker_Velocity_Control_B.qjj) {
        Parker_Velocity_Control_B.qp1jj = 1;
      } else {
        Parker_Velocity_Control_B.qp1jj = 2;
        Parker_Velocity_Control_B.qp1q = Parker_Velocity_Control_B.qjj;
      }
    }

    switch (Parker_Velocity_Control_B.qp1jj) {
     case 1:
      Parker_Velocity_Control_B.rt =
        Parker_Velocity_Control_B.e[Parker_Velocity_Control_B.i_n];
      Parker_Velocity_Control_B.e[Parker_Velocity_Control_B.i_n] = 0.0;
      Parker_Velocity_Control_B.qjj = Parker_Velocity_Control_B.i_n;
      while (Parker_Velocity_Control_B.qjj + 1 >= Parker_Velocity_Control_B.qp1q
             + 1) {
        Parker_Velocity_Control_xrotg
          (&Parker_Velocity_Control_B.b_s[Parker_Velocity_Control_B.qjj],
           &Parker_Velocity_Control_B.rt, &Parker_Velocity_Control_B.ztest,
           &Parker_Velocity_Control_B.sqds);
        if (Parker_Velocity_Control_B.qjj + 1 > Parker_Velocity_Control_B.qp1q +
            1) {
          Parker_Velocity_Control_B.emm1 =
            Parker_Velocity_Control_B.e[Parker_Velocity_Control_B.qjj - 1];
          Parker_Velocity_Control_B.rt = Parker_Velocity_Control_B.emm1 *
            -Parker_Velocity_Control_B.sqds;
          Parker_Velocity_Control_B.e[Parker_Velocity_Control_B.qjj - 1] =
            Parker_Velocity_Control_B.emm1 * Parker_Velocity_Control_B.ztest;
        }

        Parker_Velocity_Control_xrot(Parker_Velocity_Control_B.Vf, 6 *
          Parker_Velocity_Control_B.qjj + 1, 6 * (Parker_Velocity_Control_B.i_n
          + 1) + 1, Parker_Velocity_Control_B.ztest,
          Parker_Velocity_Control_B.sqds);
        Parker_Velocity_Control_B.qjj--;
      }
      break;

     case 2:
      Parker_Velocity_Control_B.rt =
        Parker_Velocity_Control_B.e[Parker_Velocity_Control_B.qp1q - 1];
      Parker_Velocity_Control_B.e[Parker_Velocity_Control_B.qp1q - 1] = 0.0;
      Parker_Velocity_Control_B.qjj = Parker_Velocity_Control_B.qp1q;
      while (Parker_Velocity_Control_B.qjj + 1 <= Parker_Velocity_Control_B.i_n
             + 2) {
        Parker_Velocity_Control_xrotg
          (&Parker_Velocity_Control_B.b_s[Parker_Velocity_Control_B.qjj],
           &Parker_Velocity_Control_B.rt, &Parker_Velocity_Control_B.ztest,
           &Parker_Velocity_Control_B.sqds);
        Parker_Velocity_Control_B.rt = -Parker_Velocity_Control_B.sqds *
          Parker_Velocity_Control_B.e[Parker_Velocity_Control_B.qjj];
        Parker_Velocity_Control_B.e[Parker_Velocity_Control_B.qjj] *=
          Parker_Velocity_Control_B.ztest;
        Parker_Velocity_Control_xrot(U, 6 * Parker_Velocity_Control_B.qjj + 1, 6
          * (Parker_Velocity_Control_B.qp1q - 1) + 1,
          Parker_Velocity_Control_B.ztest, Parker_Velocity_Control_B.sqds);
        Parker_Velocity_Control_B.qjj++;
      }
      break;

     case 3:
      Parker_Velocity_Control_B.sqds =
        Parker_Velocity_Control_B.b_s[Parker_Velocity_Control_B.i_n + 1];
      Parker_Velocity_Control_B.ztest = fabs(Parker_Velocity_Control_B.sqds);
      Parker_Velocity_Control_B.rt = fabs
        (Parker_Velocity_Control_B.b_s[Parker_Velocity_Control_B.i_n]);
      if ((Parker_Velocity_Control_B.ztest > Parker_Velocity_Control_B.rt) ||
          rtIsNaN(Parker_Velocity_Control_B.rt)) {
        Parker_Velocity_Control_B.rt = Parker_Velocity_Control_B.ztest;
      }

      Parker_Velocity_Control_B.ztest = fabs
        (Parker_Velocity_Control_B.e[Parker_Velocity_Control_B.i_n]);
      if ((Parker_Velocity_Control_B.rt > Parker_Velocity_Control_B.ztest) ||
          rtIsNaN(Parker_Velocity_Control_B.ztest)) {
        Parker_Velocity_Control_B.ztest = Parker_Velocity_Control_B.rt;
      }

      Parker_Velocity_Control_B.rt = fabs
        (Parker_Velocity_Control_B.b_s[Parker_Velocity_Control_B.qp1q]);
      if ((Parker_Velocity_Control_B.ztest > Parker_Velocity_Control_B.rt) ||
          rtIsNaN(Parker_Velocity_Control_B.rt)) {
        Parker_Velocity_Control_B.rt = Parker_Velocity_Control_B.ztest;
      }

      Parker_Velocity_Control_B.ztest = fabs
        (Parker_Velocity_Control_B.e[Parker_Velocity_Control_B.qp1q]);
      if ((Parker_Velocity_Control_B.rt > Parker_Velocity_Control_B.ztest) ||
          rtIsNaN(Parker_Velocity_Control_B.ztest)) {
        Parker_Velocity_Control_B.ztest = Parker_Velocity_Control_B.rt;
      }

      Parker_Velocity_Control_B.rt = Parker_Velocity_Control_B.sqds /
        Parker_Velocity_Control_B.ztest;
      Parker_Velocity_Control_B.smm1 =
        Parker_Velocity_Control_B.b_s[Parker_Velocity_Control_B.i_n] /
        Parker_Velocity_Control_B.ztest;
      Parker_Velocity_Control_B.emm1 =
        Parker_Velocity_Control_B.e[Parker_Velocity_Control_B.i_n] /
        Parker_Velocity_Control_B.ztest;
      Parker_Velocity_Control_B.sqds =
        Parker_Velocity_Control_B.b_s[Parker_Velocity_Control_B.qp1q] /
        Parker_Velocity_Control_B.ztest;
      Parker_Velocity_Control_B.smm1 = ((Parker_Velocity_Control_B.smm1 +
        Parker_Velocity_Control_B.rt) * (Parker_Velocity_Control_B.smm1 -
        Parker_Velocity_Control_B.rt) + Parker_Velocity_Control_B.emm1 *
        Parker_Velocity_Control_B.emm1) / 2.0;
      Parker_Velocity_Control_B.emm1 *= Parker_Velocity_Control_B.rt;
      Parker_Velocity_Control_B.emm1 *= Parker_Velocity_Control_B.emm1;
      if ((Parker_Velocity_Control_B.smm1 != 0.0) ||
          (Parker_Velocity_Control_B.emm1 != 0.0)) {
        Parker_Velocity_Control_B.shift = sqrt(Parker_Velocity_Control_B.smm1 *
          Parker_Velocity_Control_B.smm1 + Parker_Velocity_Control_B.emm1);
        if (Parker_Velocity_Control_B.smm1 < 0.0) {
          Parker_Velocity_Control_B.shift = -Parker_Velocity_Control_B.shift;
        }

        Parker_Velocity_Control_B.shift = Parker_Velocity_Control_B.emm1 /
          (Parker_Velocity_Control_B.smm1 + Parker_Velocity_Control_B.shift);
      } else {
        Parker_Velocity_Control_B.shift = 0.0;
      }

      Parker_Velocity_Control_B.rt = (Parker_Velocity_Control_B.sqds +
        Parker_Velocity_Control_B.rt) * (Parker_Velocity_Control_B.sqds -
        Parker_Velocity_Control_B.rt) + Parker_Velocity_Control_B.shift;
      Parker_Velocity_Control_B.ztest =
        Parker_Velocity_Control_B.e[Parker_Velocity_Control_B.qp1q] /
        Parker_Velocity_Control_B.ztest * Parker_Velocity_Control_B.sqds;
      Parker_Velocity_Control_B.qjj = Parker_Velocity_Control_B.qp1q + 1;
      while (Parker_Velocity_Control_B.qjj <= Parker_Velocity_Control_B.i_n + 1)
      {
        Parker_Velocity_Control_xrotg(&Parker_Velocity_Control_B.rt,
          &Parker_Velocity_Control_B.ztest, &Parker_Velocity_Control_B.sqds,
          &Parker_Velocity_Control_B.smm1);
        if (Parker_Velocity_Control_B.qjj > Parker_Velocity_Control_B.qp1q + 1)
        {
          Parker_Velocity_Control_B.e[Parker_Velocity_Control_B.qjj - 2] =
            Parker_Velocity_Control_B.rt;
        }

        Parker_Velocity_Control_B.emm1 =
          Parker_Velocity_Control_B.e[Parker_Velocity_Control_B.qjj - 1];
        Parker_Velocity_Control_B.ztest =
          Parker_Velocity_Control_B.b_s[Parker_Velocity_Control_B.qjj - 1];
        Parker_Velocity_Control_B.rt = Parker_Velocity_Control_B.ztest *
          Parker_Velocity_Control_B.sqds + Parker_Velocity_Control_B.emm1 *
          Parker_Velocity_Control_B.smm1;
        Parker_Velocity_Control_B.e[Parker_Velocity_Control_B.qjj - 1] =
          Parker_Velocity_Control_B.emm1 * Parker_Velocity_Control_B.sqds -
          Parker_Velocity_Control_B.ztest * Parker_Velocity_Control_B.smm1;
        Parker_Velocity_Control_B.ztest = Parker_Velocity_Control_B.smm1 *
          Parker_Velocity_Control_B.b_s[Parker_Velocity_Control_B.qjj];
        Parker_Velocity_Control_B.b_s[Parker_Velocity_Control_B.qjj] *=
          Parker_Velocity_Control_B.sqds;
        Parker_Velocity_Control_xrot(Parker_Velocity_Control_B.Vf, 6 *
          (Parker_Velocity_Control_B.qjj - 1) + 1, 6 *
          Parker_Velocity_Control_B.qjj + 1, Parker_Velocity_Control_B.sqds,
          Parker_Velocity_Control_B.smm1);
        Parker_Velocity_Control_xrotg(&Parker_Velocity_Control_B.rt,
          &Parker_Velocity_Control_B.ztest, &Parker_Velocity_Control_B.sqds,
          &Parker_Velocity_Control_B.smm1);
        Parker_Velocity_Control_B.b_s[Parker_Velocity_Control_B.qjj - 1] =
          Parker_Velocity_Control_B.rt;
        Parker_Velocity_Control_B.rt =
          Parker_Velocity_Control_B.e[Parker_Velocity_Control_B.qjj - 1] *
          Parker_Velocity_Control_B.sqds + Parker_Velocity_Control_B.smm1 *
          Parker_Velocity_Control_B.b_s[Parker_Velocity_Control_B.qjj];
        Parker_Velocity_Control_B.b_s[Parker_Velocity_Control_B.qjj] =
          Parker_Velocity_Control_B.e[Parker_Velocity_Control_B.qjj - 1] *
          -Parker_Velocity_Control_B.smm1 + Parker_Velocity_Control_B.sqds *
          Parker_Velocity_Control_B.b_s[Parker_Velocity_Control_B.qjj];
        Parker_Velocity_Control_B.ztest = Parker_Velocity_Control_B.smm1 *
          Parker_Velocity_Control_B.e[Parker_Velocity_Control_B.qjj];
        Parker_Velocity_Control_B.e[Parker_Velocity_Control_B.qjj] *=
          Parker_Velocity_Control_B.sqds;
        Parker_Velocity_Control_xrot(U, 6 * (Parker_Velocity_Control_B.qjj - 1)
          + 1, 6 * Parker_Velocity_Control_B.qjj + 1,
          Parker_Velocity_Control_B.sqds, Parker_Velocity_Control_B.smm1);
        Parker_Velocity_Control_B.qjj++;
      }

      Parker_Velocity_Control_B.e[Parker_Velocity_Control_B.i_n] =
        Parker_Velocity_Control_B.rt;
      Parker_Velocity_Control_B.qq++;
      break;

     default:
      if (Parker_Velocity_Control_B.b_s[Parker_Velocity_Control_B.qp1q] < 0.0) {
        Parker_Velocity_Control_B.b_s[Parker_Velocity_Control_B.qp1q] =
          -Parker_Velocity_Control_B.b_s[Parker_Velocity_Control_B.qp1q];
        Parker_Velocity_Control_xscal(-1.0, Parker_Velocity_Control_B.Vf, 6 *
          Parker_Velocity_Control_B.qp1q + 1);
      }

      Parker_Velocity_Control_B.qq = Parker_Velocity_Control_B.qp1q + 1;
      while ((Parker_Velocity_Control_B.qp1q + 1 < 6) &&
             (Parker_Velocity_Control_B.b_s[Parker_Velocity_Control_B.qp1q] <
              Parker_Velocity_Control_B.b_s[Parker_Velocity_Control_B.qq])) {
        Parker_Velocity_Control_B.rt =
          Parker_Velocity_Control_B.b_s[Parker_Velocity_Control_B.qp1q];
        Parker_Velocity_Control_B.b_s[Parker_Velocity_Control_B.qp1q] =
          Parker_Velocity_Control_B.b_s[Parker_Velocity_Control_B.qq];
        Parker_Velocity_Control_B.b_s[Parker_Velocity_Control_B.qq] =
          Parker_Velocity_Control_B.rt;
        Parker_Velocity_Control_xswap(Parker_Velocity_Control_B.Vf, 6 *
          Parker_Velocity_Control_B.qp1q + 1, 6 *
          (Parker_Velocity_Control_B.qp1q + 1) + 1);
        Parker_Velocity_Control_xswap(U, 6 * Parker_Velocity_Control_B.qp1q + 1,
          6 * (Parker_Velocity_Control_B.qp1q + 1) + 1);
        Parker_Velocity_Control_B.qp1q = Parker_Velocity_Control_B.qq;
        Parker_Velocity_Control_B.qq++;
      }

      Parker_Velocity_Control_B.qq = 0;
      Parker_Velocity_Control_B.i_n--;
      break;
    }
  }

  for (Parker_Velocity_Control_B.i_n = 0; Parker_Velocity_Control_B.i_n < 6;
       Parker_Velocity_Control_B.i_n++) {
    s[Parker_Velocity_Control_B.i_n] =
      Parker_Velocity_Control_B.b_s[Parker_Velocity_Control_B.i_n];
    for (Parker_Velocity_Control_B.qq = 0; Parker_Velocity_Control_B.qq < 6;
         Parker_Velocity_Control_B.qq++) {
      V[Parker_Velocity_Control_B.qq + 6 * Parker_Velocity_Control_B.i_n] =
        Parker_Velocity_Control_B.Vf[6 * Parker_Velocity_Control_B.i_n +
        Parker_Velocity_Control_B.qq];
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

static void Parker_Velocity__rt_invd3x3_snf(const real_T u[9], real_T y[9])
{
  memcpy(&Parker_Velocity_Control_B.x[0], &u[0], 9U * sizeof(real_T));
  Parker_Velocity_Control_B.p1 = 1;
  Parker_Velocity_Control_B.p2 = 3;
  Parker_Velocity_Control_B.p3 = 6;
  Parker_Velocity_Control_B.absx11 = fabs(u[0]);
  Parker_Velocity_Control_B.absx21 = fabs(u[1]);
  Parker_Velocity_Control_B.absx31 = fabs(u[2]);
  if ((Parker_Velocity_Control_B.absx21 > Parker_Velocity_Control_B.absx11) &&
      (Parker_Velocity_Control_B.absx21 > Parker_Velocity_Control_B.absx31)) {
    Parker_Velocity_Control_B.p1 = 4;
    Parker_Velocity_Control_B.p2 = 0;
    Parker_Velocity_Control_B.x[0] = u[1];
    Parker_Velocity_Control_B.x[1] = u[0];
    Parker_Velocity_Control_B.x[3] = u[4];
    Parker_Velocity_Control_B.x[4] = u[3];
    Parker_Velocity_Control_B.x[6] = u[7];
    Parker_Velocity_Control_B.x[7] = u[6];
  } else {
    if (Parker_Velocity_Control_B.absx31 > Parker_Velocity_Control_B.absx11) {
      Parker_Velocity_Control_B.p1 = 7;
      Parker_Velocity_Control_B.p3 = 0;
      Parker_Velocity_Control_B.x[2] = Parker_Velocity_Control_B.x[0];
      Parker_Velocity_Control_B.x[0] = u[2];
      Parker_Velocity_Control_B.x[5] = Parker_Velocity_Control_B.x[3];
      Parker_Velocity_Control_B.x[3] = u[5];
      Parker_Velocity_Control_B.x[8] = Parker_Velocity_Control_B.x[6];
      Parker_Velocity_Control_B.x[6] = u[8];
    }
  }

  Parker_Velocity_Control_B.absx31 = Parker_Velocity_Control_B.x[1] /
    Parker_Velocity_Control_B.x[0];
  Parker_Velocity_Control_B.x[1] = Parker_Velocity_Control_B.absx31;
  Parker_Velocity_Control_B.absx11 = Parker_Velocity_Control_B.x[2] /
    Parker_Velocity_Control_B.x[0];
  Parker_Velocity_Control_B.x[2] = Parker_Velocity_Control_B.absx11;
  Parker_Velocity_Control_B.x[4] -= Parker_Velocity_Control_B.absx31 *
    Parker_Velocity_Control_B.x[3];
  Parker_Velocity_Control_B.x[5] -= Parker_Velocity_Control_B.absx11 *
    Parker_Velocity_Control_B.x[3];
  Parker_Velocity_Control_B.x[7] -= Parker_Velocity_Control_B.absx31 *
    Parker_Velocity_Control_B.x[6];
  Parker_Velocity_Control_B.x[8] -= Parker_Velocity_Control_B.absx11 *
    Parker_Velocity_Control_B.x[6];
  if (fabs(Parker_Velocity_Control_B.x[5]) > fabs(Parker_Velocity_Control_B.x[4]))
  {
    Parker_Velocity_Control_B.itmp = Parker_Velocity_Control_B.p2;
    Parker_Velocity_Control_B.p2 = Parker_Velocity_Control_B.p3;
    Parker_Velocity_Control_B.p3 = Parker_Velocity_Control_B.itmp;
    Parker_Velocity_Control_B.x[1] = Parker_Velocity_Control_B.absx11;
    Parker_Velocity_Control_B.x[2] = Parker_Velocity_Control_B.absx31;
    Parker_Velocity_Control_B.absx11 = Parker_Velocity_Control_B.x[4];
    Parker_Velocity_Control_B.x[4] = Parker_Velocity_Control_B.x[5];
    Parker_Velocity_Control_B.x[5] = Parker_Velocity_Control_B.absx11;
    Parker_Velocity_Control_B.absx11 = Parker_Velocity_Control_B.x[7];
    Parker_Velocity_Control_B.x[7] = Parker_Velocity_Control_B.x[8];
    Parker_Velocity_Control_B.x[8] = Parker_Velocity_Control_B.absx11;
  }

  Parker_Velocity_Control_B.absx31 = Parker_Velocity_Control_B.x[5] /
    Parker_Velocity_Control_B.x[4];
  Parker_Velocity_Control_B.x[8] -= Parker_Velocity_Control_B.absx31 *
    Parker_Velocity_Control_B.x[7];
  Parker_Velocity_Control_B.absx11 = (Parker_Velocity_Control_B.absx31 *
    Parker_Velocity_Control_B.x[1] - Parker_Velocity_Control_B.x[2]) /
    Parker_Velocity_Control_B.x[8];
  Parker_Velocity_Control_B.absx21 = -(Parker_Velocity_Control_B.x[7] *
    Parker_Velocity_Control_B.absx11 + Parker_Velocity_Control_B.x[1]) /
    Parker_Velocity_Control_B.x[4];
  y[Parker_Velocity_Control_B.p1 - 1] = ((1.0 - Parker_Velocity_Control_B.x[3] *
    Parker_Velocity_Control_B.absx21) - Parker_Velocity_Control_B.x[6] *
    Parker_Velocity_Control_B.absx11) / Parker_Velocity_Control_B.x[0];
  y[Parker_Velocity_Control_B.p1] = Parker_Velocity_Control_B.absx21;
  y[Parker_Velocity_Control_B.p1 + 1] = Parker_Velocity_Control_B.absx11;
  Parker_Velocity_Control_B.absx11 = -Parker_Velocity_Control_B.absx31 /
    Parker_Velocity_Control_B.x[8];
  Parker_Velocity_Control_B.absx21 = (1.0 - Parker_Velocity_Control_B.x[7] *
    Parker_Velocity_Control_B.absx11) / Parker_Velocity_Control_B.x[4];
  y[Parker_Velocity_Control_B.p2] = -(Parker_Velocity_Control_B.x[3] *
    Parker_Velocity_Control_B.absx21 + Parker_Velocity_Control_B.x[6] *
    Parker_Velocity_Control_B.absx11) / Parker_Velocity_Control_B.x[0];
  y[Parker_Velocity_Control_B.p2 + 1] = Parker_Velocity_Control_B.absx21;
  y[Parker_Velocity_Control_B.p2 + 2] = Parker_Velocity_Control_B.absx11;
  Parker_Velocity_Control_B.absx11 = 1.0 / Parker_Velocity_Control_B.x[8];
  Parker_Velocity_Control_B.absx21 = -Parker_Velocity_Control_B.x[7] *
    Parker_Velocity_Control_B.absx11 / Parker_Velocity_Control_B.x[4];
  y[Parker_Velocity_Control_B.p3] = -(Parker_Velocity_Control_B.x[3] *
    Parker_Velocity_Control_B.absx21 + Parker_Velocity_Control_B.x[6] *
    Parker_Velocity_Control_B.absx11) / Parker_Velocity_Control_B.x[0];
  y[Parker_Velocity_Control_B.p3 + 1] = Parker_Velocity_Control_B.absx21;
  y[Parker_Velocity_Control_B.p3 + 2] = Parker_Velocity_Control_B.absx11;
}

static void P_SystemCore_rele_h(const MoveArm_Speed_Parker_Velocity_T *obj)
{
  if ((obj->isInitialized == 1) && obj->isSetupComplete) {
    command_dynamixel_speed(0.0, 0.0, 0.0);
  }
}

static void P_SystemCore_dele_g(const MoveArm_Speed_Parker_Velocity_T *obj)
{
  P_SystemCore_rele_h(obj);
}

static void Parker_Veloci_matlabCodegenHa_g(MoveArm_Speed_Parker_Velocity_T *obj)
{
  if (!obj->matlabCodegenIsDeleted) {
    obj->matlabCodegenIsDeleted = true;
    P_SystemCore_dele_g(obj);
  }
}

static void Parker_Vel_SystemCore_release_i(const
  codertarget_linux_blocks_Digi_T *obj)
{
  if ((obj->isInitialized == 1) && obj->isSetupComplete) {
    MW_gpioTerminate(10U);
  }
}

static void Parker_Velo_SystemCore_delete_i(const
  codertarget_linux_blocks_Digi_T *obj)
{
  Parker_Vel_SystemCore_release_i(obj);
}

static void matlabCodegenHandle_matlabCod_i(codertarget_linux_blocks_Digi_T *obj)
{
  if (!obj->matlabCodegenIsDeleted) {
    obj->matlabCodegenIsDeleted = true;
    Parker_Velo_SystemCore_delete_i(obj);
  }
}

static void Parker_Ve_SystemCore_release_ir(const
  codertarget_linux_blocks_Digi_T *obj)
{
  if ((obj->isInitialized == 1) && obj->isSetupComplete) {
    MW_gpioTerminate(26U);
  }
}

static void Parker_Vel_SystemCore_delete_ir(const
  codertarget_linux_blocks_Digi_T *obj)
{
  Parker_Ve_SystemCore_release_ir(obj);
}

static void matlabCodegenHandle_matlabCo_ir(codertarget_linux_blocks_Digi_T *obj)
{
  if (!obj->matlabCodegenIsDeleted) {
    obj->matlabCodegenIsDeleted = true;
    Parker_Vel_SystemCore_delete_ir(obj);
  }
}

static void Parker_Veloci_matlabCodegenHa_h(ReadArm_Position_Rates_Parker_T *obj)
{
  if (!obj->matlabCodegenIsDeleted) {
    obj->matlabCodegenIsDeleted = true;
  }
}

static void Parker_Veloc_SystemCore_release(const
  InitializeForSpeed_Parker_Vel_T *obj)
{
  if ((obj->isInitialized == 1) && obj->isSetupComplete) {
    terminate_dynamixel();
  }
}

static void Parker_Veloci_SystemCore_delete(const
  InitializeForSpeed_Parker_Vel_T *obj)
{
  Parker_Veloc_SystemCore_release(obj);
}

static void matlabCodegenHandle_matlabCodeg(InitializeForSpeed_Parker_Vel_T *obj)
{
  if (!obj->matlabCodegenIsDeleted) {
    obj->matlabCodegenIsDeleted = true;
    Parker_Veloci_SystemCore_delete(obj);
  }
}

static void Parker_Veloci_matlabCodegenHa_m(IdentifyPlatform_Parker_Veloc_T *obj)
{
  if (!obj->matlabCodegenIsDeleted) {
    obj->matlabCodegenIsDeleted = true;
  }
}

static void Parker_V_SystemCore_release_iri(const
  raspi_internal_PWMBlock_Parke_T *obj)
{
  if ((obj->isInitialized == 1) && obj->isSetupComplete) {
    EXT_PWMBlock_terminate(obj->PinNumber);
  }
}

static void Parker_Ve_SystemCore_delete_iri(const
  raspi_internal_PWMBlock_Parke_T *obj)
{
  Parker_V_SystemCore_release_iri(obj);
}

static void matlabCodegenHandle_matlabC_iri(raspi_internal_PWMBlock_Parke_T *obj)
{
  if (!obj->matlabCodegenIsDeleted) {
    obj->matlabCodegenIsDeleted = true;
    Parker_Ve_SystemCore_delete_iri(obj);
  }
}

static void SystemCore_release_iriacmixebgr(const
  codertarget_linux_blocks_Di_i_T *obj)
{
  if ((obj->isInitialized == 1) && obj->isSetupComplete) {
    MW_gpioTerminate(24U);
  }
}

static void SystemCore_delete_iriacmixebgr(const codertarget_linux_blocks_Di_i_T
  *obj)
{
  SystemCore_release_iriacmixebgr(obj);
}

static void matlabCodegenHandl_iriacmixebgr(codertarget_linux_blocks_Di_i_T *obj)
{
  if (!obj->matlabCodegenIsDeleted) {
    obj->matlabCodegenIsDeleted = true;
    SystemCore_delete_iriacmixebgr(obj);
  }
}

static void SystemCore_releas_iriacmixebgrh(const
  codertarget_linux_blocks_Digi_T *obj)
{
  if ((obj->isInitialized == 1) && obj->isSetupComplete) {
    MW_gpioTerminate(21U);
  }
}

static void SystemCore_delete_iriacmixebgrh(const
  codertarget_linux_blocks_Digi_T *obj)
{
  SystemCore_releas_iriacmixebgrh(obj);
}

static void matlabCodegenHand_iriacmixebgrh(codertarget_linux_blocks_Digi_T *obj)
{
  if (!obj->matlabCodegenIsDeleted) {
    obj->matlabCodegenIsDeleted = true;
    SystemCore_delete_iriacmixebgrh(obj);
  }
}

static void SystemCore_rele_iriacmixebgrhtp(const
  codertarget_linux_blocks_Digi_T *obj)
{
  if ((obj->isInitialized == 1) && obj->isSetupComplete) {
    MW_gpioTerminate(25U);
  }
}

static void SystemCore_dele_iriacmixebgrhtp(const
  codertarget_linux_blocks_Digi_T *obj)
{
  SystemCore_rele_iriacmixebgrhtp(obj);
}

static void matlabCodegenHa_iriacmixebgrhtp(codertarget_linux_blocks_Digi_T *obj)
{
  if (!obj->matlabCodegenIsDeleted) {
    obj->matlabCodegenIsDeleted = true;
    SystemCore_dele_iriacmixebgrhtp(obj);
  }
}

// Model step function for TID0
void Parker_Velocity_Control_step0(void) // Sample time: [0.0s, 0.0s]
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
    //   Constant: '<S456>/Constant'
    //   DataStoreRead: '<S16>/Data Store Read'
    //   DataStoreRead: '<S20>/Universal_Time'
    //   If: '<S16>/This IF block determines whether or not to run the BLACK sim//exp'

    if (Parker_Velocity_Control_DW.Univ_Time <
        Parker_Velocity_Control_P.Phase0_End) {
      // Outputs for IfAction SubSystem: '<Root>/Phase #0:  Wait for Synchronization' incorporates:
      //   ActionPort: '<S10>/Action Port'

      Phase0WaitforSynchronizatio
        (&Parker_Velocity_Control_P.Phase0WaitforSynchronization,
         &Parker_Velocity_Control_DW.BLACK_Fx,
         &Parker_Velocity_Control_DW.BLACK_Fy,
         &Parker_Velocity_Control_DW.BLACK_Tz,
         &Parker_Velocity_Control_DW.BLUE_Fx,
         &Parker_Velocity_Control_DW.BLUE_Fy,
         &Parker_Velocity_Control_DW.BLUE_Tz,
         &Parker_Velocity_Control_DW.Float_State,
         &Parker_Velocity_Control_DW.RED_Fx, &Parker_Velocity_Control_DW.RED_Fy,
         &Parker_Velocity_Control_DW.RED_Tz,
         &Parker_Velocity_Control_DW.RED_Tz_Elbow,
         &Parker_Velocity_Control_DW.RED_Tz_RW,
         &Parker_Velocity_Control_DW.RED_Tz_Shoulder,
         &Parker_Velocity_Control_DW.RED_Tz_Wrist,
         &Parker_Velocity_Control_DW.WhoAmI);

      // End of Outputs for SubSystem: '<Root>/Phase #0:  Wait for Synchronization' 
    } else if (Parker_Velocity_Control_DW.Univ_Time <
               Parker_Velocity_Control_P.Phase1_End) {
      // Outputs for IfAction SubSystem: '<Root>/Phase #1:  Start Floating ' incorporates:
      //   ActionPort: '<S11>/Action Port'

      Phase0WaitforSynchronizatio(&Parker_Velocity_Control_P.Phase1StartFloating,
        &Parker_Velocity_Control_DW.BLACK_Fx,
        &Parker_Velocity_Control_DW.BLACK_Fy,
        &Parker_Velocity_Control_DW.BLACK_Tz,
        &Parker_Velocity_Control_DW.BLUE_Fx, &Parker_Velocity_Control_DW.BLUE_Fy,
        &Parker_Velocity_Control_DW.BLUE_Tz,
        &Parker_Velocity_Control_DW.Float_State,
        &Parker_Velocity_Control_DW.RED_Fx, &Parker_Velocity_Control_DW.RED_Fy,
        &Parker_Velocity_Control_DW.RED_Tz,
        &Parker_Velocity_Control_DW.RED_Tz_Elbow,
        &Parker_Velocity_Control_DW.RED_Tz_RW,
        &Parker_Velocity_Control_DW.RED_Tz_Shoulder,
        &Parker_Velocity_Control_DW.RED_Tz_Wrist,
        &Parker_Velocity_Control_DW.WhoAmI);

      // End of Outputs for SubSystem: '<Root>/Phase #1:  Start Floating '
    } else if (Parker_Velocity_Control_DW.Univ_Time <
               Parker_Velocity_Control_P.Phase2_End) {
      // Outputs for IfAction SubSystem: '<Root>/Phase #2:  Move to  Initial Position' incorporates:
      //   ActionPort: '<S12>/Action Port'

      // If: '<S12>/This IF block determines whether or not to run the BLACK sim//exp' incorporates:
      //   Constant: '<S151>/Constant'
      //   DataStoreRead: '<S12>/Data Store Read'

      if ((Parker_Velocity_Control_DW.WhoAmI == 2.0) ||
          (Parker_Velocity_Control_P.simMode == 1.0)) {
        // Outputs for IfAction SubSystem: '<S12>/Change BLACK Behavior' incorporates:
        //   ActionPort: '<S148>/Action Port'

        // Sum: '<S148>/Subtract' incorporates:
        //   Constant: '<S148>/Desired Px (BLACK)'
        //   DataStoreRead: '<S148>/BLACK_Px'

        Parker_Velocity_Control_B.Subtract_o =
          Parker_Velocity_Control_P.init_states_BLACK[0] -
          Parker_Velocity_Control_DW.BLACK_Px;

        // Delay: '<S158>/Delay1'
        if (Parker_Velocity_Control_DW.icLoad_ns != 0) {
          Parker_Velocity_Control_DW.Delay1_DSTATE_k =
            Parker_Velocity_Control_B.Subtract_o;
        }

        // Sum: '<S158>/Sum6' incorporates:
        //   Delay: '<S158>/Delay1'

        Parker_Velocity_Control_B.Sum6_l = Parker_Velocity_Control_B.Subtract_o
          - Parker_Velocity_Control_DW.Delay1_DSTATE_k;

        // If: '<S158>/if we went through a "step"' incorporates:
        //   Inport: '<S159>/In1'

        if (Parker_Velocity_Control_B.Sum6_l != 0.0) {
          // Outputs for IfAction SubSystem: '<S158>/Hold this value' incorporates:
          //   ActionPort: '<S159>/Action Port'

          Parker_Velocity_Control_B.In1_ej = Parker_Velocity_Control_B.Sum6_l;

          // End of Outputs for SubSystem: '<S158>/Hold this value'
        }

        // End of If: '<S158>/if we went through a "step"'

        // Sum: '<S153>/Sum3' incorporates:
        //   DataStoreWrite: '<S148>/BLACK_Fx'
        //   Gain: '<S153>/kd_xb'
        //   Gain: '<S153>/kp_xb'
        //   Gain: '<S158>/divide by delta T'

        Parker_Velocity_Control_DW.BLACK_Fx = 1.0 /
          Parker_Velocity_Control_P.baseRate * Parker_Velocity_Control_B.In1_ej *
          Parker_Velocity_Control_P.Kd_xb + Parker_Velocity_Control_P.Kp_xb *
          Parker_Velocity_Control_B.Subtract_o;

        // Sum: '<S148>/Subtract1' incorporates:
        //   Constant: '<S148>/Desired Py (BLACK)'
        //   DataStoreRead: '<S148>/BLACK_Py '

        Parker_Velocity_Control_B.Sum6_l =
          Parker_Velocity_Control_P.init_states_BLACK[1] -
          Parker_Velocity_Control_DW.BLACK_Py;

        // Delay: '<S160>/Delay1'
        if (Parker_Velocity_Control_DW.icLoad_a != 0) {
          Parker_Velocity_Control_DW.Delay1_DSTATE_o =
            Parker_Velocity_Control_B.Sum6_l;
        }

        // Sum: '<S160>/Sum6' incorporates:
        //   Delay: '<S160>/Delay1'

        Parker_Velocity_Control_B.Sum6_d = Parker_Velocity_Control_B.Sum6_l -
          Parker_Velocity_Control_DW.Delay1_DSTATE_o;

        // If: '<S160>/if we went through a "step"' incorporates:
        //   Inport: '<S161>/In1'

        if (Parker_Velocity_Control_B.Sum6_d != 0.0) {
          // Outputs for IfAction SubSystem: '<S160>/Hold this value' incorporates:
          //   ActionPort: '<S161>/Action Port'

          Parker_Velocity_Control_B.In1_g = Parker_Velocity_Control_B.Sum6_d;

          // End of Outputs for SubSystem: '<S160>/Hold this value'
        }

        // End of If: '<S160>/if we went through a "step"'

        // Sum: '<S154>/Sum3' incorporates:
        //   DataStoreWrite: '<S148>/BLACK_Fy'
        //   Gain: '<S154>/kd_yb'
        //   Gain: '<S154>/kp_yb'
        //   Gain: '<S160>/divide by delta T'

        Parker_Velocity_Control_DW.BLACK_Fy = 1.0 /
          Parker_Velocity_Control_P.baseRate * Parker_Velocity_Control_B.In1_g *
          Parker_Velocity_Control_P.Kd_yb + Parker_Velocity_Control_P.Kp_yb *
          Parker_Velocity_Control_B.Sum6_l;

        // MATLAB Function: '<S155>/MATLAB Function2' incorporates:
        //   Constant: '<S148>/Desired Attitude (BLACK)'

        Parker_Ve_MATLABFunction2_o(Parker_Velocity_Control_P.init_states_BLACK
          [2], &Parker_Velocity_Control_B.sf_MATLABFunction2_o);

        // MATLAB Function: '<S155>/MATLAB Function3' incorporates:
        //   DataStoreRead: '<S148>/BLACK_Rz'

        Parker_Velo_MATLABFunction3(Parker_Velocity_Control_DW.BLACK_Rz,
          &Parker_Velocity_Control_B.sf_MATLABFunction3_k);

        // Sum: '<S155>/Subtract2'
        Parker_Velocity_Control_B.rtb_Subtract2_f_g[0] =
          Parker_Velocity_Control_B.sf_MATLABFunction2_o.Ox[0] -
          Parker_Velocity_Control_B.sf_MATLABFunction3_k.Ox[0];
        Parker_Velocity_Control_B.rtb_Subtract2_f_g[1] =
          Parker_Velocity_Control_B.sf_MATLABFunction2_o.Ox[1] -
          Parker_Velocity_Control_B.sf_MATLABFunction3_k.Ox[1];

        // MATLAB Function: '<S155>/MATLAB Function4'
        Parker_Velo_MATLABFunction4
          (Parker_Velocity_Control_B.sf_MATLABFunction2_o.Oy,
           Parker_Velocity_Control_B.rtb_Subtract2_f_g,
           &Parker_Velocity_Control_B.sf_MATLABFunction4_j);

        // Delay: '<S156>/Delay1'
        if (Parker_Velocity_Control_DW.icLoad_h != 0) {
          Parker_Velocity_Control_DW.Delay1_DSTATE_lg =
            Parker_Velocity_Control_B.sf_MATLABFunction4_j.e_out;
        }

        // Sum: '<S156>/Sum6' incorporates:
        //   Delay: '<S156>/Delay1'

        Parker_Velocity_Control_B.Sum6_d =
          Parker_Velocity_Control_B.sf_MATLABFunction4_j.e_out -
          Parker_Velocity_Control_DW.Delay1_DSTATE_lg;

        // If: '<S156>/if we went through a "step"' incorporates:
        //   Inport: '<S157>/In1'

        if (Parker_Velocity_Control_B.Sum6_d != 0.0) {
          // Outputs for IfAction SubSystem: '<S156>/Hold this value' incorporates:
          //   ActionPort: '<S157>/Action Port'

          Parker_Velocity_Control_B.In1_d = Parker_Velocity_Control_B.Sum6_d;

          // End of Outputs for SubSystem: '<S156>/Hold this value'
        }

        // End of If: '<S156>/if we went through a "step"'

        // Sum: '<S152>/Sum3' incorporates:
        //   DataStoreWrite: '<S148>/BLACK_Tz'
        //   Gain: '<S152>/kd_tb'
        //   Gain: '<S152>/kp_tb'
        //   Gain: '<S156>/divide by delta T'

        Parker_Velocity_Control_DW.BLACK_Tz = 1.0 /
          Parker_Velocity_Control_P.baseRate * Parker_Velocity_Control_B.In1_d *
          Parker_Velocity_Control_P.Kd_tb + Parker_Velocity_Control_P.Kp_tb *
          Parker_Velocity_Control_B.sf_MATLABFunction4_j.e_out;

        // DataStoreWrite: '<S148>/Data Store Write3' incorporates:
        //   Constant: '<S148>/Puck State'

        Parker_Velocity_Control_DW.Float_State =
          Parker_Velocity_Control_P.PuckState_Value;

        // Update for Delay: '<S158>/Delay1'
        Parker_Velocity_Control_DW.icLoad_ns = 0U;
        Parker_Velocity_Control_DW.Delay1_DSTATE_k =
          Parker_Velocity_Control_B.Subtract_o;

        // Update for Delay: '<S160>/Delay1'
        Parker_Velocity_Control_DW.icLoad_a = 0U;
        Parker_Velocity_Control_DW.Delay1_DSTATE_o =
          Parker_Velocity_Control_B.Sum6_l;

        // Update for Delay: '<S156>/Delay1'
        Parker_Velocity_Control_DW.icLoad_h = 0U;
        Parker_Velocity_Control_DW.Delay1_DSTATE_lg =
          Parker_Velocity_Control_B.sf_MATLABFunction4_j.e_out;

        // End of Outputs for SubSystem: '<S12>/Change BLACK Behavior'
      }

      // End of If: '<S12>/This IF block determines whether or not to run the BLACK sim//exp' 

      // If: '<S12>/This IF block determines whether or not to run the BLUE sim//exp' incorporates:
      //   Constant: '<S151>/Constant'
      //   DataStoreRead: '<S12>/Data Store Read'

      if ((Parker_Velocity_Control_DW.WhoAmI == 3.0) ||
          (Parker_Velocity_Control_P.simMode == 1.0)) {
        // Outputs for IfAction SubSystem: '<S12>/Change BLUE Behavior' incorporates:
        //   ActionPort: '<S149>/Action Port'

        // Sum: '<S149>/Subtract' incorporates:
        //   Constant: '<S149>/Desired Px (BLUE)'
        //   DataStoreRead: '<S149>/BLUE_Px'

        Parker_Velocity_Control_B.Subtract_o =
          Parker_Velocity_Control_P.init_states_BLUE[0] -
          Parker_Velocity_Control_DW.BLUE_Px;

        // Delay: '<S171>/Delay1'
        if (Parker_Velocity_Control_DW.icLoad_dy != 0) {
          Parker_Velocity_Control_DW.Delay1_DSTATE_fb =
            Parker_Velocity_Control_B.Subtract_o;
        }

        // Sum: '<S171>/Sum6' incorporates:
        //   Delay: '<S171>/Delay1'

        Parker_Velocity_Control_B.Sum6_l = Parker_Velocity_Control_B.Subtract_o
          - Parker_Velocity_Control_DW.Delay1_DSTATE_fb;

        // If: '<S171>/if we went through a "step"' incorporates:
        //   Inport: '<S172>/In1'

        if (Parker_Velocity_Control_B.Sum6_l != 0.0) {
          // Outputs for IfAction SubSystem: '<S171>/Hold this value' incorporates:
          //   ActionPort: '<S172>/Action Port'

          Parker_Velocity_Control_B.In1_l = Parker_Velocity_Control_B.Sum6_l;

          // End of Outputs for SubSystem: '<S171>/Hold this value'
        }

        // End of If: '<S171>/if we went through a "step"'

        // Sum: '<S166>/Sum3' incorporates:
        //   DataStoreWrite: '<S149>/BLUE_Fx'
        //   Gain: '<S166>/kd_xb'
        //   Gain: '<S166>/kp_xb'
        //   Gain: '<S171>/divide by delta T'

        Parker_Velocity_Control_DW.BLUE_Fx = 1.0 /
          Parker_Velocity_Control_P.baseRate * Parker_Velocity_Control_B.In1_l *
          Parker_Velocity_Control_P.Kd_xblue +
          Parker_Velocity_Control_P.Kp_xblue *
          Parker_Velocity_Control_B.Subtract_o;

        // Sum: '<S149>/Subtract1' incorporates:
        //   Constant: '<S149>/Desired Py (BLUE)'
        //   DataStoreRead: '<S149>/BLUE_Py '

        Parker_Velocity_Control_B.Sum6_l =
          Parker_Velocity_Control_P.init_states_BLUE[1] -
          Parker_Velocity_Control_DW.BLUE_Py;

        // Delay: '<S173>/Delay1'
        if (Parker_Velocity_Control_DW.icLoad_n != 0) {
          Parker_Velocity_Control_DW.Delay1_DSTATE_l =
            Parker_Velocity_Control_B.Sum6_l;
        }

        // Sum: '<S173>/Sum6' incorporates:
        //   Delay: '<S173>/Delay1'

        Parker_Velocity_Control_B.Sum6_d = Parker_Velocity_Control_B.Sum6_l -
          Parker_Velocity_Control_DW.Delay1_DSTATE_l;

        // If: '<S173>/if we went through a "step"' incorporates:
        //   Inport: '<S174>/In1'

        if (Parker_Velocity_Control_B.Sum6_d != 0.0) {
          // Outputs for IfAction SubSystem: '<S173>/Hold this value' incorporates:
          //   ActionPort: '<S174>/Action Port'

          Parker_Velocity_Control_B.In1_f = Parker_Velocity_Control_B.Sum6_d;

          // End of Outputs for SubSystem: '<S173>/Hold this value'
        }

        // End of If: '<S173>/if we went through a "step"'

        // Sum: '<S167>/Sum3' incorporates:
        //   DataStoreWrite: '<S149>/BLUE_Fy'
        //   Gain: '<S167>/kd_yb'
        //   Gain: '<S167>/kp_yb'
        //   Gain: '<S173>/divide by delta T'

        Parker_Velocity_Control_DW.BLUE_Fy = 1.0 /
          Parker_Velocity_Control_P.baseRate * Parker_Velocity_Control_B.In1_f *
          Parker_Velocity_Control_P.Kd_yblue +
          Parker_Velocity_Control_P.Kp_yblue * Parker_Velocity_Control_B.Sum6_l;

        // MATLAB Function: '<S168>/MATLAB Function2' incorporates:
        //   Constant: '<S149>/Desired Attitude (BLUE)'

        Parker_Ve_MATLABFunction2_o(Parker_Velocity_Control_P.init_states_BLUE[2],
          &Parker_Velocity_Control_B.sf_MATLABFunction2_m);

        // MATLAB Function: '<S168>/MATLAB Function3' incorporates:
        //   DataStoreRead: '<S149>/BLUE_Rz'

        Parker_Velo_MATLABFunction3(Parker_Velocity_Control_DW.BLUE_Rz,
          &Parker_Velocity_Control_B.sf_MATLABFunction3_c);

        // Sum: '<S168>/Subtract2'
        Parker_Velocity_Control_B.rtb_Subtract2_f_g[0] =
          Parker_Velocity_Control_B.sf_MATLABFunction2_m.Ox[0] -
          Parker_Velocity_Control_B.sf_MATLABFunction3_c.Ox[0];
        Parker_Velocity_Control_B.rtb_Subtract2_f_g[1] =
          Parker_Velocity_Control_B.sf_MATLABFunction2_m.Ox[1] -
          Parker_Velocity_Control_B.sf_MATLABFunction3_c.Ox[1];

        // MATLAB Function: '<S168>/MATLAB Function4'
        Parker_Velo_MATLABFunction4
          (Parker_Velocity_Control_B.sf_MATLABFunction2_m.Oy,
           Parker_Velocity_Control_B.rtb_Subtract2_f_g,
           &Parker_Velocity_Control_B.sf_MATLABFunction4_c);

        // Delay: '<S169>/Delay1'
        if (Parker_Velocity_Control_DW.icLoad_bp != 0) {
          Parker_Velocity_Control_DW.Delay1_DSTATE_n =
            Parker_Velocity_Control_B.sf_MATLABFunction4_c.e_out;
        }

        // Sum: '<S169>/Sum6' incorporates:
        //   Delay: '<S169>/Delay1'

        Parker_Velocity_Control_B.Sum6_d =
          Parker_Velocity_Control_B.sf_MATLABFunction4_c.e_out -
          Parker_Velocity_Control_DW.Delay1_DSTATE_n;

        // If: '<S169>/if we went through a "step"' incorporates:
        //   Inport: '<S170>/In1'

        if (Parker_Velocity_Control_B.Sum6_d != 0.0) {
          // Outputs for IfAction SubSystem: '<S169>/Hold this value' incorporates:
          //   ActionPort: '<S170>/Action Port'

          Parker_Velocity_Control_B.In1_fj = Parker_Velocity_Control_B.Sum6_d;

          // End of Outputs for SubSystem: '<S169>/Hold this value'
        }

        // End of If: '<S169>/if we went through a "step"'

        // Sum: '<S165>/Sum3' incorporates:
        //   DataStoreWrite: '<S149>/BLUE_Tz'
        //   Gain: '<S165>/kd_tb'
        //   Gain: '<S165>/kp_tb'
        //   Gain: '<S169>/divide by delta T'

        Parker_Velocity_Control_DW.BLUE_Tz = 1.0 /
          Parker_Velocity_Control_P.baseRate * Parker_Velocity_Control_B.In1_fj *
          Parker_Velocity_Control_P.Kd_tblue +
          Parker_Velocity_Control_P.Kp_tblue *
          Parker_Velocity_Control_B.sf_MATLABFunction4_c.e_out;

        // DataStoreWrite: '<S149>/Data Store Write3' incorporates:
        //   Constant: '<S149>/Puck State'

        Parker_Velocity_Control_DW.Float_State =
          Parker_Velocity_Control_P.PuckState_Value_o;

        // Update for Delay: '<S171>/Delay1'
        Parker_Velocity_Control_DW.icLoad_dy = 0U;
        Parker_Velocity_Control_DW.Delay1_DSTATE_fb =
          Parker_Velocity_Control_B.Subtract_o;

        // Update for Delay: '<S173>/Delay1'
        Parker_Velocity_Control_DW.icLoad_n = 0U;
        Parker_Velocity_Control_DW.Delay1_DSTATE_l =
          Parker_Velocity_Control_B.Sum6_l;

        // Update for Delay: '<S169>/Delay1'
        Parker_Velocity_Control_DW.icLoad_bp = 0U;
        Parker_Velocity_Control_DW.Delay1_DSTATE_n =
          Parker_Velocity_Control_B.sf_MATLABFunction4_c.e_out;

        // End of Outputs for SubSystem: '<S12>/Change BLUE Behavior'
      }

      // End of If: '<S12>/This IF block determines whether or not to run the BLUE sim//exp' 

      // If: '<S12>/This IF block determines whether or not to run the RED sim//exp ' incorporates:
      //   Constant: '<S151>/Constant'
      //   DataStoreRead: '<S12>/Data Store Read'

      if ((Parker_Velocity_Control_DW.WhoAmI == 1.0) ||
          (Parker_Velocity_Control_P.simMode == 1.0)) {
        // Outputs for IfAction SubSystem: '<S12>/Change RED Behavior' incorporates:
        //   ActionPort: '<S150>/Action Port'

        // DataStoreWrite: '<S150>/RED_Tz_RW1' incorporates:
        //   Constant: '<S150>/Constant2'

        Parker_Velocity_Control_DW.RED_Tz_RW =
          Parker_Velocity_Control_P.Constant2_Value;

        // DataStoreWrite: '<S150>/Data Store Write3' incorporates:
        //   Constant: '<S150>/Puck State'

        Parker_Velocity_Control_DW.Float_State =
          Parker_Velocity_Control_P.PuckState_Value_l;

        // If: '<S150>/If' incorporates:
        //   Constant: '<S150>/Constant4'

        if ((Parker_Velocity_Control_P.platformSelection == 4.0) ||
            (Parker_Velocity_Control_P.platformSelection == 5.0) ||
            (Parker_Velocity_Control_P.platformSelection == 10.0) ||
            (Parker_Velocity_Control_P.platformSelection == 11.0)) {
          // Outputs for IfAction SubSystem: '<S150>/RED+ARM' incorporates:
          //   ActionPort: '<S179>/Action Port'

          // DataStoreWrite: '<S179>/ARM_EL_Tz' incorporates:
          //   Constant: '<S179>/Constant1'

          Parker_Velocity_Control_DW.RED_Tz_Elbow =
            Parker_Velocity_Control_P.Constant1_Value;

          // DataStoreWrite: '<S179>/ARM_SH_Tz' incorporates:
          //   Constant: '<S179>/Constant1'

          Parker_Velocity_Control_DW.RED_Tz_Shoulder =
            Parker_Velocity_Control_P.Constant1_Value;

          // DataStoreWrite: '<S179>/ARM_WR_Tz' incorporates:
          //   Constant: '<S179>/Constant1'

          Parker_Velocity_Control_DW.RED_Tz_Wrist =
            Parker_Velocity_Control_P.Constant1_Value;

          // End of Outputs for SubSystem: '<S150>/RED+ARM'
        } else {
          // Outputs for IfAction SubSystem: '<S150>/RED' incorporates:
          //   ActionPort: '<S178>/Action Port'

          // DataStoreWrite: '<S178>/RED_Fx1' incorporates:
          //   Constant: '<S178>/Constant7'

          Parker_Velocity_Control_DW.RED_Tz_Shoulder =
            Parker_Velocity_Control_P.Constant7_Value;

          // DataStoreWrite: '<S178>/RED_Fy1' incorporates:
          //   Constant: '<S178>/Constant7'

          Parker_Velocity_Control_DW.RED_Tz_Elbow =
            Parker_Velocity_Control_P.Constant7_Value;

          // DataStoreWrite: '<S178>/RED_Tz1' incorporates:
          //   Constant: '<S178>/Constant7'

          Parker_Velocity_Control_DW.RED_Tz_Wrist =
            Parker_Velocity_Control_P.Constant7_Value;

          // End of Outputs for SubSystem: '<S150>/RED'
        }

        // End of If: '<S150>/If'
        // End of Outputs for SubSystem: '<S12>/Change RED Behavior'
      }

      // End of If: '<S12>/This IF block determines whether or not to run the RED sim//exp ' 
      // End of Outputs for SubSystem: '<Root>/Phase #2:  Move to  Initial Position' 
    } else if (Parker_Velocity_Control_DW.Univ_Time <
               Parker_Velocity_Control_P.Phase3_End) {
      // Outputs for IfAction SubSystem: '<Root>/Phase #3: Experiment' incorporates:
      //   ActionPort: '<S13>/Action Port'

      // If: '<S13>/This IF block determines whether or not to run the BLACK sim//exp' incorporates:
      //   Constant: '<S183>/Constant'
      //   DataStoreRead: '<S13>/Data Store Read'

      if ((Parker_Velocity_Control_DW.WhoAmI == 2.0) ||
          (Parker_Velocity_Control_P.simMode == 1.0)) {
        // Outputs for IfAction SubSystem: '<S13>/Change BLACK Behavior' incorporates:
        //   ActionPort: '<S180>/Action Port'

        // If: '<S180>/Experiment Sub-Phases' incorporates:
        //   Constant: '<S180>/Constant1'
        //   Constant: '<S180>/Constant2'
        //   Constant: '<S180>/Constant3'
        //   Constant: '<S180>/Constant4'
        //   DataStoreRead: '<S188>/Universal_Time'

        if (Parker_Velocity_Control_DW.Univ_Time <
            Parker_Velocity_Control_P.Phase3_SubPhase1_End) {
          // Outputs for IfAction SubSystem: '<S180>/Sub-Phase #1' incorporates:
          //   ActionPort: '<S184>/Action Port'

          Parker_Velocity_C_SubPhase1(&Parker_Velocity_Control_B.SubPhase1,
            &Parker_Velocity_Control_DW.SubPhase1,
            &Parker_Velocity_Control_P.SubPhase1,
            &Parker_Velocity_Control_DW.BLACK_Fx,
            &Parker_Velocity_Control_DW.BLACK_Fy,
            &Parker_Velocity_Control_DW.BLACK_Px,
            &Parker_Velocity_Control_DW.BLACK_Py,
            &Parker_Velocity_Control_DW.BLACK_Rz,
            &Parker_Velocity_Control_DW.BLACK_Tz,
            &Parker_Velocity_Control_DW.Float_State,
            &Parker_Velocity_Control_DW.Univ_Time);

          // End of Outputs for SubSystem: '<S180>/Sub-Phase #1'
        } else if (Parker_Velocity_Control_DW.Univ_Time <
                   Parker_Velocity_Control_P.Phase3_SubPhase2_End) {
          // Outputs for IfAction SubSystem: '<S180>/Sub-Phase #2 ' incorporates:
          //   ActionPort: '<S185>/Action Port'

          Parker_Velocity_C_SubPhase1(&Parker_Velocity_Control_B.SubPhase2,
            &Parker_Velocity_Control_DW.SubPhase2,
            &Parker_Velocity_Control_P.SubPhase2,
            &Parker_Velocity_Control_DW.BLACK_Fx,
            &Parker_Velocity_Control_DW.BLACK_Fy,
            &Parker_Velocity_Control_DW.BLACK_Px,
            &Parker_Velocity_Control_DW.BLACK_Py,
            &Parker_Velocity_Control_DW.BLACK_Rz,
            &Parker_Velocity_Control_DW.BLACK_Tz,
            &Parker_Velocity_Control_DW.Float_State,
            &Parker_Velocity_Control_DW.Univ_Time);

          // End of Outputs for SubSystem: '<S180>/Sub-Phase #2 '
        } else if (Parker_Velocity_Control_DW.Univ_Time <
                   Parker_Velocity_Control_P.Phase3_SubPhase3_End) {
          // Outputs for IfAction SubSystem: '<S180>/Sub-Phase #3 ' incorporates:
          //   ActionPort: '<S186>/Action Port'

          Parker_Velocity_C_SubPhase1(&Parker_Velocity_Control_B.SubPhase3,
            &Parker_Velocity_Control_DW.SubPhase3,
            &Parker_Velocity_Control_P.SubPhase3,
            &Parker_Velocity_Control_DW.BLACK_Fx,
            &Parker_Velocity_Control_DW.BLACK_Fy,
            &Parker_Velocity_Control_DW.BLACK_Px,
            &Parker_Velocity_Control_DW.BLACK_Py,
            &Parker_Velocity_Control_DW.BLACK_Rz,
            &Parker_Velocity_Control_DW.BLACK_Tz,
            &Parker_Velocity_Control_DW.Float_State,
            &Parker_Velocity_Control_DW.Univ_Time);

          // End of Outputs for SubSystem: '<S180>/Sub-Phase #3 '
        } else {
          if (Parker_Velocity_Control_DW.Univ_Time <
              Parker_Velocity_Control_P.Phase3_SubPhase4_End) {
            // Outputs for IfAction SubSystem: '<S180>/Sub-Phase #4' incorporates:
            //   ActionPort: '<S187>/Action Port'

            Parker_Velocity_C_SubPhase1(&Parker_Velocity_Control_B.SubPhase4,
              &Parker_Velocity_Control_DW.SubPhase4,
              &Parker_Velocity_Control_P.SubPhase4,
              &Parker_Velocity_Control_DW.BLACK_Fx,
              &Parker_Velocity_Control_DW.BLACK_Fy,
              &Parker_Velocity_Control_DW.BLACK_Px,
              &Parker_Velocity_Control_DW.BLACK_Py,
              &Parker_Velocity_Control_DW.BLACK_Rz,
              &Parker_Velocity_Control_DW.BLACK_Tz,
              &Parker_Velocity_Control_DW.Float_State,
              &Parker_Velocity_Control_DW.Univ_Time);

            // End of Outputs for SubSystem: '<S180>/Sub-Phase #4'
          }
        }

        // End of If: '<S180>/Experiment Sub-Phases'
        // End of Outputs for SubSystem: '<S13>/Change BLACK Behavior'
      }

      // End of If: '<S13>/This IF block determines whether or not to run the BLACK sim//exp' 

      // If: '<S13>/This IF block determines whether or not to run the BLUE sim//exp' incorporates:
      //   Constant: '<S183>/Constant'
      //   DataStoreRead: '<S13>/Data Store Read'

      if ((Parker_Velocity_Control_DW.WhoAmI == 3.0) ||
          (Parker_Velocity_Control_P.simMode == 1.0)) {
        // Outputs for IfAction SubSystem: '<S13>/Change BLUE Behavior' incorporates:
        //   ActionPort: '<S181>/Action Port'

        // If: '<S181>/Experiment Sub-Phases' incorporates:
        //   Constant: '<S181>/Constant1'
        //   Constant: '<S181>/Constant2'
        //   Constant: '<S181>/Constant3'
        //   Constant: '<S181>/Constant4'
        //   DataStoreRead: '<S261>/Universal_Time'

        if (Parker_Velocity_Control_DW.Univ_Time <
            Parker_Velocity_Control_P.Phase3_SubPhase1_End) {
          // Outputs for IfAction SubSystem: '<S181>/Sub-Phase #1' incorporates:
          //   ActionPort: '<S257>/Action Port'

          Parker_Velocity_SubPhase1_a(&Parker_Velocity_Control_B.SubPhase1_a,
            &Parker_Velocity_Control_DW.SubPhase1_a,
            &Parker_Velocity_Control_P.SubPhase1_a,
            &Parker_Velocity_Control_DW.BLACK_Px,
            &Parker_Velocity_Control_DW.BLACK_Py,
            &Parker_Velocity_Control_DW.BLUE_Fx,
            &Parker_Velocity_Control_DW.BLUE_Fy,
            &Parker_Velocity_Control_DW.BLUE_Px,
            &Parker_Velocity_Control_DW.BLUE_Py,
            &Parker_Velocity_Control_DW.BLUE_Rz,
            &Parker_Velocity_Control_DW.BLUE_Tz,
            &Parker_Velocity_Control_DW.Float_State,
            &Parker_Velocity_Control_DW.Univ_Time);

          // End of Outputs for SubSystem: '<S181>/Sub-Phase #1'
        } else if (Parker_Velocity_Control_DW.Univ_Time <
                   Parker_Velocity_Control_P.Phase3_SubPhase2_End) {
          // Outputs for IfAction SubSystem: '<S181>/Sub-Phase #2 ' incorporates:
          //   ActionPort: '<S258>/Action Port'

          Parker_Velocity_SubPhase1_a(&Parker_Velocity_Control_B.SubPhase2_m,
            &Parker_Velocity_Control_DW.SubPhase2_m,
            &Parker_Velocity_Control_P.SubPhase2_m,
            &Parker_Velocity_Control_DW.BLACK_Px,
            &Parker_Velocity_Control_DW.BLACK_Py,
            &Parker_Velocity_Control_DW.BLUE_Fx,
            &Parker_Velocity_Control_DW.BLUE_Fy,
            &Parker_Velocity_Control_DW.BLUE_Px,
            &Parker_Velocity_Control_DW.BLUE_Py,
            &Parker_Velocity_Control_DW.BLUE_Rz,
            &Parker_Velocity_Control_DW.BLUE_Tz,
            &Parker_Velocity_Control_DW.Float_State,
            &Parker_Velocity_Control_DW.Univ_Time);

          // End of Outputs for SubSystem: '<S181>/Sub-Phase #2 '
        } else if (Parker_Velocity_Control_DW.Univ_Time <
                   Parker_Velocity_Control_P.Phase3_SubPhase3_End) {
          // Outputs for IfAction SubSystem: '<S181>/Sub-Phase #3 ' incorporates:
          //   ActionPort: '<S259>/Action Port'

          Parker_Velocity_SubPhase1_a(&Parker_Velocity_Control_B.SubPhase3_p,
            &Parker_Velocity_Control_DW.SubPhase3_p,
            &Parker_Velocity_Control_P.SubPhase3_p,
            &Parker_Velocity_Control_DW.BLACK_Px,
            &Parker_Velocity_Control_DW.BLACK_Py,
            &Parker_Velocity_Control_DW.BLUE_Fx,
            &Parker_Velocity_Control_DW.BLUE_Fy,
            &Parker_Velocity_Control_DW.BLUE_Px,
            &Parker_Velocity_Control_DW.BLUE_Py,
            &Parker_Velocity_Control_DW.BLUE_Rz,
            &Parker_Velocity_Control_DW.BLUE_Tz,
            &Parker_Velocity_Control_DW.Float_State,
            &Parker_Velocity_Control_DW.Univ_Time);

          // End of Outputs for SubSystem: '<S181>/Sub-Phase #3 '
        } else {
          if (Parker_Velocity_Control_DW.Univ_Time <
              Parker_Velocity_Control_P.Phase3_SubPhase4_End) {
            // Outputs for IfAction SubSystem: '<S181>/Sub-Phase #4' incorporates:
            //   ActionPort: '<S260>/Action Port'

            Parker_Velocity_SubPhase1_a(&Parker_Velocity_Control_B.SubPhase4_f,
              &Parker_Velocity_Control_DW.SubPhase4_f,
              &Parker_Velocity_Control_P.SubPhase4_f,
              &Parker_Velocity_Control_DW.BLACK_Px,
              &Parker_Velocity_Control_DW.BLACK_Py,
              &Parker_Velocity_Control_DW.BLUE_Fx,
              &Parker_Velocity_Control_DW.BLUE_Fy,
              &Parker_Velocity_Control_DW.BLUE_Px,
              &Parker_Velocity_Control_DW.BLUE_Py,
              &Parker_Velocity_Control_DW.BLUE_Rz,
              &Parker_Velocity_Control_DW.BLUE_Tz,
              &Parker_Velocity_Control_DW.Float_State,
              &Parker_Velocity_Control_DW.Univ_Time);

            // End of Outputs for SubSystem: '<S181>/Sub-Phase #4'
          }
        }

        // End of If: '<S181>/Experiment Sub-Phases'
        // End of Outputs for SubSystem: '<S13>/Change BLUE Behavior'
      }

      // End of If: '<S13>/This IF block determines whether or not to run the BLUE sim//exp' 

      // If: '<S13>/This IF block determines whether or not to run the RED sim//exp ' incorporates:
      //   Constant: '<S183>/Constant'
      //   DataStoreRead: '<S13>/Data Store Read'

      if ((Parker_Velocity_Control_DW.WhoAmI == 1.0) ||
          (Parker_Velocity_Control_P.simMode == 1.0)) {
        // Outputs for IfAction SubSystem: '<S13>/Change RED Behavior' incorporates:
        //   ActionPort: '<S182>/Action Port'

        // If: '<S182>/Experiment Sub-Phases' incorporates:
        //   Constant: '<S182>/Constant1'
        //   Constant: '<S182>/Constant2'
        //   Constant: '<S182>/Constant3'
        //   Constant: '<S182>/Constant4'
        //   DataStoreRead: '<S338>/Universal_Time'

        if (Parker_Velocity_Control_DW.Univ_Time <
            Parker_Velocity_Control_P.Phase3_SubPhase1_End) {
          // Outputs for IfAction SubSystem: '<S182>/Sub-Phase #1' incorporates:
          //   ActionPort: '<S334>/Action Port'

          Parker_Velocity_SubPhase1_j(&Parker_Velocity_Control_P.SubPhase1_j,
            &Parker_Velocity_Control_DW.Float_State,
            &Parker_Velocity_Control_DW.RED_Tz_Elbow,
            &Parker_Velocity_Control_DW.RED_Tz_RW,
            &Parker_Velocity_Control_DW.RED_Tz_Shoulder,
            &Parker_Velocity_Control_DW.RED_Tz_Wrist);

          // End of Outputs for SubSystem: '<S182>/Sub-Phase #1'
        } else if (Parker_Velocity_Control_DW.Univ_Time <
                   Parker_Velocity_Control_P.Phase3_SubPhase2_End) {
          // Outputs for IfAction SubSystem: '<S182>/Sub-Phase #2 ' incorporates:
          //   ActionPort: '<S335>/Action Port'

          Parker_Velocity_SubPhase1_j(&Parker_Velocity_Control_P.SubPhase2_g,
            &Parker_Velocity_Control_DW.Float_State,
            &Parker_Velocity_Control_DW.RED_Tz_Elbow,
            &Parker_Velocity_Control_DW.RED_Tz_RW,
            &Parker_Velocity_Control_DW.RED_Tz_Shoulder,
            &Parker_Velocity_Control_DW.RED_Tz_Wrist);

          // End of Outputs for SubSystem: '<S182>/Sub-Phase #2 '
        } else if (Parker_Velocity_Control_DW.Univ_Time <
                   Parker_Velocity_Control_P.Phase3_SubPhase3_End) {
          // Outputs for IfAction SubSystem: '<S182>/Sub-Phase #3 ' incorporates:
          //   ActionPort: '<S336>/Action Port'

          // DataStoreWrite: '<S336>/RED_Tz_RW' incorporates:
          //   Constant: '<S336>/Constant4'

          Parker_Velocity_Control_DW.RED_Tz_RW =
            Parker_Velocity_Control_P.Constant4_Value;

          // DataStoreWrite: '<S336>/Data Store Write3' incorporates:
          //   Constant: '<S336>/Puck State'

          Parker_Velocity_Control_DW.Float_State =
            Parker_Velocity_Control_P.PuckState_Value_lm;

          // If: '<S336>/If' incorporates:
          //   Constant: '<S336>/Constant1'

          if ((Parker_Velocity_Control_P.platformSelection == 4.0) ||
              (Parker_Velocity_Control_P.platformSelection == 5.0) ||
              (Parker_Velocity_Control_P.platformSelection == 10.0) ||
              (Parker_Velocity_Control_P.platformSelection == 11.0)) {
            // Outputs for IfAction SubSystem: '<S336>/RED+ARM' incorporates:
            //   ActionPort: '<S344>/Action Port'

            // DataStoreWrite: '<S344>/ARM_EL_Tz' incorporates:
            //   Constant: '<S344>/Constant1'

            Parker_Velocity_Control_DW.RED_Tz_Elbow =
              Parker_Velocity_Control_P.Constant1_Value_n;

            // DataStoreWrite: '<S344>/ARM_SH_Tz' incorporates:
            //   Constant: '<S344>/Constant1'

            Parker_Velocity_Control_DW.RED_Tz_Shoulder =
              Parker_Velocity_Control_P.Constant1_Value_n;

            // DataStoreWrite: '<S344>/ARM_WR_Tz' incorporates:
            //   Constant: '<S344>/Constant1'

            Parker_Velocity_Control_DW.RED_Tz_Wrist =
              Parker_Velocity_Control_P.Constant1_Value_n;

            // End of Outputs for SubSystem: '<S336>/RED+ARM'
          } else {
            // Outputs for IfAction SubSystem: '<S336>/RED' incorporates:
            //   ActionPort: '<S343>/Action Port'

            // DataStoreWrite: '<S343>/RED_Fx1' incorporates:
            //   Constant: '<S343>/Constant7'

            Parker_Velocity_Control_DW.RED_Tz_Shoulder =
              Parker_Velocity_Control_P.Constant7_Value_k;

            // DataStoreWrite: '<S343>/RED_Fy1' incorporates:
            //   Constant: '<S343>/Constant7'

            Parker_Velocity_Control_DW.RED_Tz_Elbow =
              Parker_Velocity_Control_P.Constant7_Value_k;

            // DataStoreWrite: '<S343>/RED_Tz1' incorporates:
            //   Constant: '<S343>/Constant7'

            Parker_Velocity_Control_DW.RED_Tz_Wrist =
              Parker_Velocity_Control_P.Constant7_Value_k;

            // End of Outputs for SubSystem: '<S336>/RED'
          }

          // End of If: '<S336>/If'
          // End of Outputs for SubSystem: '<S182>/Sub-Phase #3 '
        } else {
          if (Parker_Velocity_Control_DW.Univ_Time <
              Parker_Velocity_Control_P.Phase3_SubPhase4_End) {
            // Outputs for IfAction SubSystem: '<S182>/Sub-Phase #4' incorporates:
            //   ActionPort: '<S337>/Action Port'

            // DataStoreWrite: '<S337>/RED_Tz_RW' incorporates:
            //   Constant: '<S337>/Constant1'

            Parker_Velocity_Control_DW.RED_Tz_RW =
              Parker_Velocity_Control_P.Constant1_Value_f;

            // DataStoreWrite: '<S337>/Data Store Write3' incorporates:
            //   Constant: '<S337>/Puck State'

            Parker_Velocity_Control_DW.Float_State =
              Parker_Velocity_Control_P.PuckState_Value_f;

            // If: '<S337>/If' incorporates:
            //   Constant: '<S337>/Constant3'

            if ((Parker_Velocity_Control_P.platformSelection == 4.0) ||
                (Parker_Velocity_Control_P.platformSelection == 5.0) ||
                (Parker_Velocity_Control_P.platformSelection == 10.0) ||
                (Parker_Velocity_Control_P.platformSelection == 11.0)) {
              // Outputs for IfAction SubSystem: '<S337>/RED+ARM' incorporates:
              //   ActionPort: '<S346>/Action Port'

              // MATLABSystem: '<S346>/Send Speed Commands to  Manipulator' incorporates:
              //   Constant: '<S346>/Constant1'
              //   Constant: '<S346>/Constant3'
              //   Constant: '<S346>/Constant4'
              //   Gain: '<S346>/Gain'

              command_dynamixel_speed(Parker_Velocity_Control_P.Gain_Gain *
                Parker_Velocity_Control_P.Constant1_Value_h,
                Parker_Velocity_Control_P.Constant3_Value,
                Parker_Velocity_Control_P.Constant4_Value_l);

              // End of Outputs for SubSystem: '<S337>/RED+ARM'
            } else {
              // Outputs for IfAction SubSystem: '<S337>/RED' incorporates:
              //   ActionPort: '<S345>/Action Port'

              // DataStoreWrite: '<S345>/RED_Fx1' incorporates:
              //   Constant: '<S345>/Constant7'

              Parker_Velocity_Control_DW.RED_Tz_Shoulder =
                Parker_Velocity_Control_P.Constant7_Value_f;

              // DataStoreWrite: '<S345>/RED_Fy1' incorporates:
              //   Constant: '<S345>/Constant7'

              Parker_Velocity_Control_DW.RED_Tz_Elbow =
                Parker_Velocity_Control_P.Constant7_Value_f;

              // DataStoreWrite: '<S345>/RED_Tz1' incorporates:
              //   Constant: '<S345>/Constant7'

              Parker_Velocity_Control_DW.RED_Tz_Wrist =
                Parker_Velocity_Control_P.Constant7_Value_f;

              // End of Outputs for SubSystem: '<S337>/RED'
            }

            // End of If: '<S337>/If'
            // End of Outputs for SubSystem: '<S182>/Sub-Phase #4'
          }
        }

        // End of If: '<S182>/Experiment Sub-Phases'
        // End of Outputs for SubSystem: '<S13>/Change RED Behavior'
      }

      // End of If: '<S13>/This IF block determines whether or not to run the RED sim//exp ' 
      // End of Outputs for SubSystem: '<Root>/Phase #3: Experiment'
    } else if (Parker_Velocity_Control_DW.Univ_Time <
               Parker_Velocity_Control_P.Phase4_End) {
      // Outputs for IfAction SubSystem: '<Root>/Phase #4:  Return Home' incorporates:
      //   ActionPort: '<S14>/Action Port'

      // If: '<S14>/This IF block determines whether or not to run the BLACK sim//exp' incorporates:
      //   Constant: '<S350>/Constant'
      //   DataStoreRead: '<S14>/Data Store Read'

      if ((Parker_Velocity_Control_DW.WhoAmI == 2.0) ||
          (Parker_Velocity_Control_P.simMode == 1.0)) {
        // Outputs for IfAction SubSystem: '<S14>/Change BLACK Behavior' incorporates:
        //   ActionPort: '<S347>/Action Port'

        // Sum: '<S347>/Subtract' incorporates:
        //   Constant: '<S347>/Desired Px (BLACK)'
        //   DataStoreRead: '<S347>/BLACK_Px'

        Parker_Velocity_Control_B.Subtract_o =
          Parker_Velocity_Control_P.home_states_BLACK[0] -
          Parker_Velocity_Control_DW.BLACK_Px;

        // Delay: '<S357>/Delay1'
        if (Parker_Velocity_Control_DW.icLoad_e != 0) {
          Parker_Velocity_Control_DW.Delay1_DSTATE_db =
            Parker_Velocity_Control_B.Subtract_o;
        }

        // Sum: '<S357>/Sum6' incorporates:
        //   Delay: '<S357>/Delay1'

        Parker_Velocity_Control_B.Sum6_l = Parker_Velocity_Control_B.Subtract_o
          - Parker_Velocity_Control_DW.Delay1_DSTATE_db;

        // If: '<S357>/if we went through a "step"' incorporates:
        //   Inport: '<S358>/In1'

        if (Parker_Velocity_Control_B.Sum6_l != 0.0) {
          // Outputs for IfAction SubSystem: '<S357>/Hold this value' incorporates:
          //   ActionPort: '<S358>/Action Port'

          Parker_Velocity_Control_B.In1_b = Parker_Velocity_Control_B.Sum6_l;

          // End of Outputs for SubSystem: '<S357>/Hold this value'
        }

        // End of If: '<S357>/if we went through a "step"'

        // Sum: '<S352>/Sum3' incorporates:
        //   DataStoreWrite: '<S347>/BLACK_Fx'
        //   Gain: '<S352>/kd_xb'
        //   Gain: '<S352>/kp_xb'
        //   Gain: '<S357>/divide by delta T'

        Parker_Velocity_Control_DW.BLACK_Fx = 1.0 /
          Parker_Velocity_Control_P.baseRate * Parker_Velocity_Control_B.In1_b *
          Parker_Velocity_Control_P.Kd_xb + Parker_Velocity_Control_P.Kp_xb *
          Parker_Velocity_Control_B.Subtract_o;

        // Sum: '<S347>/Subtract1' incorporates:
        //   Constant: '<S347>/Desired Py (BLACK)'
        //   DataStoreRead: '<S347>/BLACK_Py '

        Parker_Velocity_Control_B.Sum6_l =
          Parker_Velocity_Control_P.home_states_BLACK[1] -
          Parker_Velocity_Control_DW.BLACK_Py;

        // Delay: '<S359>/Delay1'
        if (Parker_Velocity_Control_DW.icLoad_fr != 0) {
          Parker_Velocity_Control_DW.Delay1_DSTATE_dz =
            Parker_Velocity_Control_B.Sum6_l;
        }

        // Sum: '<S359>/Sum6' incorporates:
        //   Delay: '<S359>/Delay1'

        Parker_Velocity_Control_B.Sum6_d = Parker_Velocity_Control_B.Sum6_l -
          Parker_Velocity_Control_DW.Delay1_DSTATE_dz;

        // If: '<S359>/if we went through a "step"' incorporates:
        //   Inport: '<S360>/In1'

        if (Parker_Velocity_Control_B.Sum6_d != 0.0) {
          // Outputs for IfAction SubSystem: '<S359>/Hold this value' incorporates:
          //   ActionPort: '<S360>/Action Port'

          Parker_Velocity_Control_B.In1_o5 = Parker_Velocity_Control_B.Sum6_d;

          // End of Outputs for SubSystem: '<S359>/Hold this value'
        }

        // End of If: '<S359>/if we went through a "step"'

        // Sum: '<S353>/Sum3' incorporates:
        //   DataStoreWrite: '<S347>/BLACK_Fy'
        //   Gain: '<S353>/kd_yb'
        //   Gain: '<S353>/kp_yb'
        //   Gain: '<S359>/divide by delta T'

        Parker_Velocity_Control_DW.BLACK_Fy = 1.0 /
          Parker_Velocity_Control_P.baseRate * Parker_Velocity_Control_B.In1_o5 *
          Parker_Velocity_Control_P.Kd_yb + Parker_Velocity_Control_P.Kp_yb *
          Parker_Velocity_Control_B.Sum6_l;

        // MATLAB Function: '<S354>/MATLAB Function2' incorporates:
        //   Constant: '<S347>/Desired Attitude (BLACK)'

        Parker_Ve_MATLABFunction2_o(Parker_Velocity_Control_P.home_states_BLACK
          [2], &Parker_Velocity_Control_B.sf_MATLABFunction2_p);

        // MATLAB Function: '<S354>/MATLAB Function3' incorporates:
        //   DataStoreRead: '<S347>/BLACK_Rz'

        Parker_Velo_MATLABFunction3(Parker_Velocity_Control_DW.BLACK_Rz,
          &Parker_Velocity_Control_B.sf_MATLABFunction3_b);

        // Sum: '<S354>/Subtract2'
        Parker_Velocity_Control_B.rtb_Subtract2_f_g[0] =
          Parker_Velocity_Control_B.sf_MATLABFunction2_p.Ox[0] -
          Parker_Velocity_Control_B.sf_MATLABFunction3_b.Ox[0];
        Parker_Velocity_Control_B.rtb_Subtract2_f_g[1] =
          Parker_Velocity_Control_B.sf_MATLABFunction2_p.Ox[1] -
          Parker_Velocity_Control_B.sf_MATLABFunction3_b.Ox[1];

        // MATLAB Function: '<S354>/MATLAB Function4'
        Parker_Velo_MATLABFunction4
          (Parker_Velocity_Control_B.sf_MATLABFunction2_p.Oy,
           Parker_Velocity_Control_B.rtb_Subtract2_f_g,
           &Parker_Velocity_Control_B.sf_MATLABFunction4_l);

        // Delay: '<S355>/Delay1'
        if (Parker_Velocity_Control_DW.icLoad_o != 0) {
          Parker_Velocity_Control_DW.Delay1_DSTATE_h =
            Parker_Velocity_Control_B.sf_MATLABFunction4_l.e_out;
        }

        // Sum: '<S355>/Sum6' incorporates:
        //   Delay: '<S355>/Delay1'

        Parker_Velocity_Control_B.Sum6_d =
          Parker_Velocity_Control_B.sf_MATLABFunction4_l.e_out -
          Parker_Velocity_Control_DW.Delay1_DSTATE_h;

        // If: '<S355>/if we went through a "step"' incorporates:
        //   Inport: '<S356>/In1'

        if (Parker_Velocity_Control_B.Sum6_d != 0.0) {
          // Outputs for IfAction SubSystem: '<S355>/Hold this value' incorporates:
          //   ActionPort: '<S356>/Action Port'

          Parker_Velocity_Control_B.In1_ei = Parker_Velocity_Control_B.Sum6_d;

          // End of Outputs for SubSystem: '<S355>/Hold this value'
        }

        // End of If: '<S355>/if we went through a "step"'

        // Sum: '<S351>/Sum3' incorporates:
        //   DataStoreWrite: '<S347>/BLACK_Tz'
        //   Gain: '<S351>/kd_tb'
        //   Gain: '<S351>/kp_tb'
        //   Gain: '<S355>/divide by delta T'

        Parker_Velocity_Control_DW.BLACK_Tz = 1.0 /
          Parker_Velocity_Control_P.baseRate * Parker_Velocity_Control_B.In1_ei *
          Parker_Velocity_Control_P.Kd_tb + Parker_Velocity_Control_P.Kp_tb *
          Parker_Velocity_Control_B.sf_MATLABFunction4_l.e_out;

        // DataStoreWrite: '<S347>/Data Store Write3' incorporates:
        //   Constant: '<S347>/Puck State'

        Parker_Velocity_Control_DW.Float_State =
          Parker_Velocity_Control_P.PuckState_Value_fj;

        // Update for Delay: '<S357>/Delay1'
        Parker_Velocity_Control_DW.icLoad_e = 0U;
        Parker_Velocity_Control_DW.Delay1_DSTATE_db =
          Parker_Velocity_Control_B.Subtract_o;

        // Update for Delay: '<S359>/Delay1'
        Parker_Velocity_Control_DW.icLoad_fr = 0U;
        Parker_Velocity_Control_DW.Delay1_DSTATE_dz =
          Parker_Velocity_Control_B.Sum6_l;

        // Update for Delay: '<S355>/Delay1'
        Parker_Velocity_Control_DW.icLoad_o = 0U;
        Parker_Velocity_Control_DW.Delay1_DSTATE_h =
          Parker_Velocity_Control_B.sf_MATLABFunction4_l.e_out;

        // End of Outputs for SubSystem: '<S14>/Change BLACK Behavior'
      }

      // End of If: '<S14>/This IF block determines whether or not to run the BLACK sim//exp' 

      // If: '<S14>/This IF block determines whether or not to run the BLUE sim//exp' incorporates:
      //   Constant: '<S350>/Constant'
      //   DataStoreRead: '<S14>/Data Store Read'

      if ((Parker_Velocity_Control_DW.WhoAmI == 3.0) ||
          (Parker_Velocity_Control_P.simMode == 1.0)) {
        // Outputs for IfAction SubSystem: '<S14>/Change BLUE Behavior' incorporates:
        //   ActionPort: '<S348>/Action Port'

        Parker_ChangeBLUEBehavior_g
          (&Parker_Velocity_Control_B.ChangeBLUEBehavior_gj,
           &Parker_Velocity_Control_DW.ChangeBLUEBehavior_gj,
           &Parker_Velocity_Control_P.ChangeBLUEBehavior_gj,
           &Parker_Velocity_Control_DW.BLUE_Fx,
           &Parker_Velocity_Control_DW.BLUE_Fy,
           &Parker_Velocity_Control_DW.BLUE_Px,
           &Parker_Velocity_Control_DW.BLUE_Py,
           &Parker_Velocity_Control_DW.BLUE_Rz,
           &Parker_Velocity_Control_DW.BLUE_Tz,
           &Parker_Velocity_Control_DW.Float_State);

        // End of Outputs for SubSystem: '<S14>/Change BLUE Behavior'
      }

      // End of If: '<S14>/This IF block determines whether or not to run the BLUE sim//exp' 

      // If: '<S14>/This IF block determines whether or not to run the RED sim//exp ' incorporates:
      //   Constant: '<S350>/Constant'
      //   DataStoreRead: '<S14>/Data Store Read'

      if ((Parker_Velocity_Control_DW.WhoAmI == 1.0) ||
          (Parker_Velocity_Control_P.simMode == 1.0)) {
        // Outputs for IfAction SubSystem: '<S14>/Change RED Behavior' incorporates:
        //   ActionPort: '<S349>/Action Port'

        // DataStoreWrite: '<S349>/RED_Tz_RW1' incorporates:
        //   Constant: '<S349>/Constant2'

        Parker_Velocity_Control_DW.RED_Tz_RW =
          Parker_Velocity_Control_P.Constant2_Value_g;

        // MATLAB Function: '<S380>/MATLAB Function2' incorporates:
        //   Constant: '<S349>/Desired Px (BLACK)2'

        Parker_Ve_MATLABFunction2_o(Parker_Velocity_Control_P.home_states_RED[2],
          &Parker_Velocity_Control_B.sf_MATLABFunction2_i);

        // MATLAB Function: '<S380>/MATLAB Function3' incorporates:
        //   DataStoreRead: '<S349>/RED_Rz'

        Parker_Velo_MATLABFunction3(Parker_Velocity_Control_DW.RED_Rz,
          &Parker_Velocity_Control_B.sf_MATLABFunction3_c1);

        // Sum: '<S380>/Subtract2'
        Parker_Velocity_Control_B.rtb_Subtract2_f_g[0] =
          Parker_Velocity_Control_B.sf_MATLABFunction2_i.Ox[0] -
          Parker_Velocity_Control_B.sf_MATLABFunction3_c1.Ox[0];
        Parker_Velocity_Control_B.rtb_Subtract2_f_g[1] =
          Parker_Velocity_Control_B.sf_MATLABFunction2_i.Ox[1] -
          Parker_Velocity_Control_B.sf_MATLABFunction3_c1.Ox[1];

        // MATLAB Function: '<S380>/MATLAB Function4'
        Parker_Velo_MATLABFunction4
          (Parker_Velocity_Control_B.sf_MATLABFunction2_i.Oy,
           Parker_Velocity_Control_B.rtb_Subtract2_f_g,
           &Parker_Velocity_Control_B.sf_MATLABFunction4_n);

        // Delay: '<S383>/Delay1'
        if (Parker_Velocity_Control_DW.icLoad_k != 0) {
          Parker_Velocity_Control_DW.Delay1_DSTATE_i =
            Parker_Velocity_Control_B.sf_MATLABFunction4_n.e_out;
        }

        // Sum: '<S383>/Sum6' incorporates:
        //   Delay: '<S383>/Delay1'

        Parker_Velocity_Control_B.Subtract_o =
          Parker_Velocity_Control_B.sf_MATLABFunction4_n.e_out -
          Parker_Velocity_Control_DW.Delay1_DSTATE_i;

        // If: '<S383>/if we went through a "step"' incorporates:
        //   Inport: '<S384>/In1'

        if (Parker_Velocity_Control_B.Subtract_o != 0.0) {
          // Outputs for IfAction SubSystem: '<S383>/Hold this value' incorporates:
          //   ActionPort: '<S384>/Action Port'

          Parker_Velocity_Control_B.In1_o = Parker_Velocity_Control_B.Subtract_o;

          // End of Outputs for SubSystem: '<S383>/Hold this value'
        }

        // End of If: '<S383>/if we went through a "step"'

        // Sum: '<S377>/Sum3' incorporates:
        //   DataStoreWrite: '<S349>/RED_Tz'
        //   Gain: '<S377>/kd_tr'
        //   Gain: '<S377>/kp_tr'
        //   Gain: '<S383>/divide by delta T'

        Parker_Velocity_Control_DW.RED_Tz = 1.0 /
          Parker_Velocity_Control_P.baseRate * Parker_Velocity_Control_B.In1_o *
          Parker_Velocity_Control_P.Kd_tr + Parker_Velocity_Control_P.Kp_tr *
          Parker_Velocity_Control_B.sf_MATLABFunction4_n.e_out;

        // Sum: '<S349>/Subtract' incorporates:
        //   Constant: '<S349>/Desired Px (BLACK)'
        //   DataStoreRead: '<S349>/RED_Px'

        Parker_Velocity_Control_B.Subtract_o =
          Parker_Velocity_Control_P.home_states_RED[0] -
          Parker_Velocity_Control_DW.RED_Px;

        // Delay: '<S385>/Delay1'
        if (Parker_Velocity_Control_DW.icLoad_du != 0) {
          Parker_Velocity_Control_DW.Delay1_DSTATE_i3 =
            Parker_Velocity_Control_B.Subtract_o;
        }

        // Sum: '<S385>/Sum6' incorporates:
        //   Delay: '<S385>/Delay1'

        Parker_Velocity_Control_B.Sum6_l = Parker_Velocity_Control_B.Subtract_o
          - Parker_Velocity_Control_DW.Delay1_DSTATE_i3;

        // If: '<S385>/if we went through a "step"' incorporates:
        //   Inport: '<S386>/In1'

        if (Parker_Velocity_Control_B.Sum6_l != 0.0) {
          // Outputs for IfAction SubSystem: '<S385>/Hold this value' incorporates:
          //   ActionPort: '<S386>/Action Port'

          Parker_Velocity_Control_B.In1_c = Parker_Velocity_Control_B.Sum6_l;

          // End of Outputs for SubSystem: '<S385>/Hold this value'
        }

        // End of If: '<S385>/if we went through a "step"'

        // Sum: '<S378>/Sum3' incorporates:
        //   DataStoreWrite: '<S349>/RED_Fx'
        //   Gain: '<S378>/kd_xr'
        //   Gain: '<S378>/kp_xr'
        //   Gain: '<S385>/divide by delta T'

        Parker_Velocity_Control_DW.RED_Fx = 1.0 /
          Parker_Velocity_Control_P.baseRate * Parker_Velocity_Control_B.In1_c *
          Parker_Velocity_Control_P.Kd_xr + Parker_Velocity_Control_P.Kp_xr *
          Parker_Velocity_Control_B.Subtract_o;

        // Sum: '<S349>/Subtract1' incorporates:
        //   Constant: '<S349>/Desired Px (BLACK)1'
        //   DataStoreRead: '<S349>/RED_Py '

        Parker_Velocity_Control_B.Sum6_l =
          Parker_Velocity_Control_P.home_states_RED[1] -
          Parker_Velocity_Control_DW.RED_Py;

        // Delay: '<S387>/Delay1'
        if (Parker_Velocity_Control_DW.icLoad_g != 0) {
          Parker_Velocity_Control_DW.Delay1_DSTATE_cl =
            Parker_Velocity_Control_B.Sum6_l;
        }

        // Sum: '<S387>/Sum6' incorporates:
        //   Delay: '<S387>/Delay1'

        Parker_Velocity_Control_B.Sum6_d = Parker_Velocity_Control_B.Sum6_l -
          Parker_Velocity_Control_DW.Delay1_DSTATE_cl;

        // If: '<S387>/if we went through a "step"' incorporates:
        //   Inport: '<S388>/In1'

        if (Parker_Velocity_Control_B.Sum6_d != 0.0) {
          // Outputs for IfAction SubSystem: '<S387>/Hold this value' incorporates:
          //   ActionPort: '<S388>/Action Port'

          Parker_Velocity_Control_B.In1_a = Parker_Velocity_Control_B.Sum6_d;

          // End of Outputs for SubSystem: '<S387>/Hold this value'
        }

        // End of If: '<S387>/if we went through a "step"'

        // Sum: '<S379>/Sum3' incorporates:
        //   DataStoreWrite: '<S349>/RED_Fy'
        //   Gain: '<S379>/kd_yr'
        //   Gain: '<S379>/kp_yr'
        //   Gain: '<S387>/divide by delta T'

        Parker_Velocity_Control_DW.RED_Fy = 1.0 /
          Parker_Velocity_Control_P.baseRate * Parker_Velocity_Control_B.In1_a *
          Parker_Velocity_Control_P.Kd_yr + Parker_Velocity_Control_P.Kp_yr *
          Parker_Velocity_Control_B.Sum6_l;

        // DataStoreWrite: '<S349>/Data Store Write3' incorporates:
        //   Constant: '<S349>/Puck State'

        Parker_Velocity_Control_DW.Float_State =
          Parker_Velocity_Control_P.PuckState_Value_od;

        // If: '<S349>/If' incorporates:
        //   Constant: '<S349>/Constant3'

        if ((Parker_Velocity_Control_P.platformSelection == 4.0) ||
            (Parker_Velocity_Control_P.platformSelection == 5.0) ||
            (Parker_Velocity_Control_P.platformSelection == 10.0) ||
            (Parker_Velocity_Control_P.platformSelection == 11.0)) {
          // Outputs for IfAction SubSystem: '<S349>/RED+ARM' incorporates:
          //   ActionPort: '<S382>/Action Port'

          Parker_Velocity_Cont_REDARM(&Parker_Velocity_Control_B.REDARM_p,
            &Parker_Velocity_Control_P.REDARM_p,
            &Parker_Velocity_Control_DW.RED_RzD_Elbow,
            &Parker_Velocity_Control_DW.RED_RzD_Shoulder,
            &Parker_Velocity_Control_DW.RED_RzD_Wrist,
            &Parker_Velocity_Control_DW.RED_Rz_Elbow,
            &Parker_Velocity_Control_DW.RED_Rz_Shoulder,
            &Parker_Velocity_Control_DW.RED_Rz_Wrist,
            &Parker_Velocity_Control_DW.RED_Tz_Elbow,
            &Parker_Velocity_Control_DW.RED_Tz_Shoulder,
            &Parker_Velocity_Control_DW.RED_Tz_Wrist);

          // End of Outputs for SubSystem: '<S349>/RED+ARM'
        } else {
          // Outputs for IfAction SubSystem: '<S349>/RED' incorporates:
          //   ActionPort: '<S381>/Action Port'

          // DataStoreWrite: '<S381>/RED_Fx1' incorporates:
          //   Constant: '<S381>/Constant7'

          Parker_Velocity_Control_DW.RED_Tz_Shoulder =
            Parker_Velocity_Control_P.Constant7_Value_m;

          // DataStoreWrite: '<S381>/RED_Fy1' incorporates:
          //   Constant: '<S381>/Constant7'

          Parker_Velocity_Control_DW.RED_Tz_Elbow =
            Parker_Velocity_Control_P.Constant7_Value_m;

          // DataStoreWrite: '<S381>/RED_Tz1' incorporates:
          //   Constant: '<S381>/Constant7'

          Parker_Velocity_Control_DW.RED_Tz_Wrist =
            Parker_Velocity_Control_P.Constant7_Value_m;

          // End of Outputs for SubSystem: '<S349>/RED'
        }

        // End of If: '<S349>/If'

        // Update for Delay: '<S383>/Delay1'
        Parker_Velocity_Control_DW.icLoad_k = 0U;
        Parker_Velocity_Control_DW.Delay1_DSTATE_i =
          Parker_Velocity_Control_B.sf_MATLABFunction4_n.e_out;

        // Update for Delay: '<S385>/Delay1'
        Parker_Velocity_Control_DW.icLoad_du = 0U;
        Parker_Velocity_Control_DW.Delay1_DSTATE_i3 =
          Parker_Velocity_Control_B.Subtract_o;

        // Update for Delay: '<S387>/Delay1'
        Parker_Velocity_Control_DW.icLoad_g = 0U;
        Parker_Velocity_Control_DW.Delay1_DSTATE_cl =
          Parker_Velocity_Control_B.Sum6_l;

        // End of Outputs for SubSystem: '<S14>/Change RED Behavior'
      }

      // End of If: '<S14>/This IF block determines whether or not to run the RED sim//exp ' 
      // End of Outputs for SubSystem: '<Root>/Phase #4:  Return Home'
    } else if (Parker_Velocity_Control_DW.Univ_Time <
               Parker_Velocity_Control_P.Phase5_End) {
      // Outputs for IfAction SubSystem: '<Root>/Phase #5:  Hold Home' incorporates:
      //   ActionPort: '<S15>/Action Port'

      // If: '<S15>/This IF block determines whether or not to run the BLACK sim//exp' incorporates:
      //   Constant: '<S403>/Constant'
      //   DataStoreRead: '<S15>/Data Store Read'

      if ((Parker_Velocity_Control_DW.WhoAmI == 2.0) ||
          (Parker_Velocity_Control_P.simMode == 1.0)) {
        // Outputs for IfAction SubSystem: '<S15>/Change BLACK Behavior' incorporates:
        //   ActionPort: '<S400>/Action Port'

        // Sum: '<S400>/Subtract' incorporates:
        //   Constant: '<S400>/Constant'
        //   DataStoreRead: '<S400>/BLACK_Px'

        Parker_Velocity_Control_B.Subtract_o =
          Parker_Velocity_Control_P.home_states_BLACK[0] -
          Parker_Velocity_Control_DW.BLACK_Px;

        // Delay: '<S410>/Delay1'
        if (Parker_Velocity_Control_DW.icLoad_d != 0) {
          Parker_Velocity_Control_DW.Delay1_DSTATE_j =
            Parker_Velocity_Control_B.Subtract_o;
        }

        // Sum: '<S410>/Sum6' incorporates:
        //   Delay: '<S410>/Delay1'

        Parker_Velocity_Control_B.Sum6_l = Parker_Velocity_Control_B.Subtract_o
          - Parker_Velocity_Control_DW.Delay1_DSTATE_j;

        // If: '<S410>/if we went through a "step"' incorporates:
        //   Inport: '<S411>/In1'

        if (Parker_Velocity_Control_B.Sum6_l != 0.0) {
          // Outputs for IfAction SubSystem: '<S410>/Hold this value' incorporates:
          //   ActionPort: '<S411>/Action Port'

          Parker_Velocity_Control_B.In1_it = Parker_Velocity_Control_B.Sum6_l;

          // End of Outputs for SubSystem: '<S410>/Hold this value'
        }

        // End of If: '<S410>/if we went through a "step"'

        // Sum: '<S405>/Sum3' incorporates:
        //   DataStoreWrite: '<S400>/BLACK_Fx'
        //   Gain: '<S405>/kd_xb'
        //   Gain: '<S405>/kp_xb'
        //   Gain: '<S410>/divide by delta T'

        Parker_Velocity_Control_DW.BLACK_Fx = 1.0 /
          Parker_Velocity_Control_P.baseRate * Parker_Velocity_Control_B.In1_it *
          Parker_Velocity_Control_P.Kd_xb + Parker_Velocity_Control_P.Kp_xb *
          Parker_Velocity_Control_B.Subtract_o;

        // Sum: '<S400>/Subtract1' incorporates:
        //   Constant: '<S400>/Constant2'
        //   DataStoreRead: '<S400>/BLACK_Py '

        Parker_Velocity_Control_B.Sum6_l =
          Parker_Velocity_Control_P.home_states_BLACK[1] -
          Parker_Velocity_Control_DW.BLACK_Py;

        // Delay: '<S412>/Delay1'
        if (Parker_Velocity_Control_DW.icLoad_f != 0) {
          Parker_Velocity_Control_DW.Delay1_DSTATE_f =
            Parker_Velocity_Control_B.Sum6_l;
        }

        // Sum: '<S412>/Sum6' incorporates:
        //   Delay: '<S412>/Delay1'

        Parker_Velocity_Control_B.Sum6_d = Parker_Velocity_Control_B.Sum6_l -
          Parker_Velocity_Control_DW.Delay1_DSTATE_f;

        // If: '<S412>/if we went through a "step"' incorporates:
        //   Inport: '<S413>/In1'

        if (Parker_Velocity_Control_B.Sum6_d != 0.0) {
          // Outputs for IfAction SubSystem: '<S412>/Hold this value' incorporates:
          //   ActionPort: '<S413>/Action Port'

          Parker_Velocity_Control_B.In1_p = Parker_Velocity_Control_B.Sum6_d;

          // End of Outputs for SubSystem: '<S412>/Hold this value'
        }

        // End of If: '<S412>/if we went through a "step"'

        // Sum: '<S406>/Sum3' incorporates:
        //   DataStoreWrite: '<S400>/BLACK_Fy'
        //   Gain: '<S406>/kd_yb'
        //   Gain: '<S406>/kp_yb'
        //   Gain: '<S412>/divide by delta T'

        Parker_Velocity_Control_DW.BLACK_Fy = 1.0 /
          Parker_Velocity_Control_P.baseRate * Parker_Velocity_Control_B.In1_p *
          Parker_Velocity_Control_P.Kd_yb + Parker_Velocity_Control_P.Kp_yb *
          Parker_Velocity_Control_B.Sum6_l;

        // MATLAB Function: '<S407>/MATLAB Function2' incorporates:
        //   Constant: '<S400>/Constant3'

        Parker_Ve_MATLABFunction2_o(Parker_Velocity_Control_P.home_states_BLACK
          [2], &Parker_Velocity_Control_B.sf_MATLABFunction2_g);

        // MATLAB Function: '<S407>/MATLAB Function3' incorporates:
        //   DataStoreRead: '<S400>/BLACK_Rz'

        Parker_Velo_MATLABFunction3(Parker_Velocity_Control_DW.BLACK_Rz,
          &Parker_Velocity_Control_B.sf_MATLABFunction3_o);

        // Sum: '<S407>/Subtract2'
        Parker_Velocity_Control_B.rtb_Subtract2_f_g[0] =
          Parker_Velocity_Control_B.sf_MATLABFunction2_g.Ox[0] -
          Parker_Velocity_Control_B.sf_MATLABFunction3_o.Ox[0];
        Parker_Velocity_Control_B.rtb_Subtract2_f_g[1] =
          Parker_Velocity_Control_B.sf_MATLABFunction2_g.Ox[1] -
          Parker_Velocity_Control_B.sf_MATLABFunction3_o.Ox[1];

        // MATLAB Function: '<S407>/MATLAB Function4'
        Parker_Velo_MATLABFunction4
          (Parker_Velocity_Control_B.sf_MATLABFunction2_g.Oy,
           Parker_Velocity_Control_B.rtb_Subtract2_f_g,
           &Parker_Velocity_Control_B.sf_MATLABFunction4_d);

        // Delay: '<S408>/Delay1'
        if (Parker_Velocity_Control_DW.icLoad_b != 0) {
          Parker_Velocity_Control_DW.Delay1_DSTATE_ci =
            Parker_Velocity_Control_B.sf_MATLABFunction4_d.e_out;
        }

        // Sum: '<S408>/Sum6' incorporates:
        //   Delay: '<S408>/Delay1'

        Parker_Velocity_Control_B.Sum6_d =
          Parker_Velocity_Control_B.sf_MATLABFunction4_d.e_out -
          Parker_Velocity_Control_DW.Delay1_DSTATE_ci;

        // If: '<S408>/if we went through a "step"' incorporates:
        //   Inport: '<S409>/In1'

        if (Parker_Velocity_Control_B.Sum6_d != 0.0) {
          // Outputs for IfAction SubSystem: '<S408>/Hold this value' incorporates:
          //   ActionPort: '<S409>/Action Port'

          Parker_Velocity_Control_B.In1_j = Parker_Velocity_Control_B.Sum6_d;

          // End of Outputs for SubSystem: '<S408>/Hold this value'
        }

        // End of If: '<S408>/if we went through a "step"'

        // Sum: '<S404>/Sum3' incorporates:
        //   DataStoreWrite: '<S400>/BLACK_Tz'
        //   Gain: '<S404>/kd_tb'
        //   Gain: '<S404>/kp_tb'
        //   Gain: '<S408>/divide by delta T'

        Parker_Velocity_Control_DW.BLACK_Tz = 1.0 /
          Parker_Velocity_Control_P.baseRate * Parker_Velocity_Control_B.In1_j *
          Parker_Velocity_Control_P.Kd_tb + Parker_Velocity_Control_P.Kp_tb *
          Parker_Velocity_Control_B.sf_MATLABFunction4_d.e_out;

        // DataStoreWrite: '<S400>/Data Store Write3' incorporates:
        //   Constant: '<S400>/Puck State'

        Parker_Velocity_Control_DW.Float_State =
          Parker_Velocity_Control_P.PuckState_Value_g;

        // Update for Delay: '<S410>/Delay1'
        Parker_Velocity_Control_DW.icLoad_d = 0U;
        Parker_Velocity_Control_DW.Delay1_DSTATE_j =
          Parker_Velocity_Control_B.Subtract_o;

        // Update for Delay: '<S412>/Delay1'
        Parker_Velocity_Control_DW.icLoad_f = 0U;
        Parker_Velocity_Control_DW.Delay1_DSTATE_f =
          Parker_Velocity_Control_B.Sum6_l;

        // Update for Delay: '<S408>/Delay1'
        Parker_Velocity_Control_DW.icLoad_b = 0U;
        Parker_Velocity_Control_DW.Delay1_DSTATE_ci =
          Parker_Velocity_Control_B.sf_MATLABFunction4_d.e_out;

        // End of Outputs for SubSystem: '<S15>/Change BLACK Behavior'
      }

      // End of If: '<S15>/This IF block determines whether or not to run the BLACK sim//exp' 

      // If: '<S15>/This IF block determines whether or not to run the BLUE sim//exp' incorporates:
      //   Constant: '<S403>/Constant'
      //   DataStoreRead: '<S15>/Data Store Read'

      if ((Parker_Velocity_Control_DW.WhoAmI == 3.0) ||
          (Parker_Velocity_Control_P.simMode == 1.0)) {
        // Outputs for IfAction SubSystem: '<S15>/Change BLUE Behavior' incorporates:
        //   ActionPort: '<S401>/Action Port'

        Parker_ChangeBLUEBehavior_g
          (&Parker_Velocity_Control_B.ChangeBLUEBehavior_b,
           &Parker_Velocity_Control_DW.ChangeBLUEBehavior_b,
           &Parker_Velocity_Control_P.ChangeBLUEBehavior_b,
           &Parker_Velocity_Control_DW.BLUE_Fx,
           &Parker_Velocity_Control_DW.BLUE_Fy,
           &Parker_Velocity_Control_DW.BLUE_Px,
           &Parker_Velocity_Control_DW.BLUE_Py,
           &Parker_Velocity_Control_DW.BLUE_Rz,
           &Parker_Velocity_Control_DW.BLUE_Tz,
           &Parker_Velocity_Control_DW.Float_State);

        // End of Outputs for SubSystem: '<S15>/Change BLUE Behavior'
      }

      // End of If: '<S15>/This IF block determines whether or not to run the BLUE sim//exp' 

      // If: '<S15>/This IF block determines whether or not to run the RED sim//exp ' incorporates:
      //   Constant: '<S403>/Constant'
      //   DataStoreRead: '<S15>/Data Store Read'

      if ((Parker_Velocity_Control_DW.WhoAmI == 1.0) ||
          (Parker_Velocity_Control_P.simMode == 1.0)) {
        // Outputs for IfAction SubSystem: '<S15>/Change RED Behavior' incorporates:
        //   ActionPort: '<S402>/Action Port'

        // DataStoreWrite: '<S402>/RED_Tz_RW' incorporates:
        //   Constant: '<S402>/Constant1'

        Parker_Velocity_Control_DW.RED_Tz_RW =
          Parker_Velocity_Control_P.Constant1_Value_k;

        // MATLAB Function: '<S433>/MATLAB Function2' incorporates:
        //   Constant: '<S402>/Desired Px (BLACK)2'

        Parker_Ve_MATLABFunction2_o(Parker_Velocity_Control_P.home_states_RED[2],
          &Parker_Velocity_Control_B.sf_MATLABFunction2_c);

        // MATLAB Function: '<S433>/MATLAB Function3' incorporates:
        //   DataStoreRead: '<S402>/RED_Rz'

        Parker_Velo_MATLABFunction3(Parker_Velocity_Control_DW.RED_Rz,
          &Parker_Velocity_Control_B.sf_MATLABFunction3_f);

        // Sum: '<S433>/Subtract2'
        Parker_Velocity_Control_B.rtb_Subtract2_f_g[0] =
          Parker_Velocity_Control_B.sf_MATLABFunction2_c.Ox[0] -
          Parker_Velocity_Control_B.sf_MATLABFunction3_f.Ox[0];
        Parker_Velocity_Control_B.rtb_Subtract2_f_g[1] =
          Parker_Velocity_Control_B.sf_MATLABFunction2_c.Ox[1] -
          Parker_Velocity_Control_B.sf_MATLABFunction3_f.Ox[1];

        // MATLAB Function: '<S433>/MATLAB Function4'
        Parker_Velo_MATLABFunction4
          (Parker_Velocity_Control_B.sf_MATLABFunction2_c.Oy,
           Parker_Velocity_Control_B.rtb_Subtract2_f_g,
           &Parker_Velocity_Control_B.sf_MATLABFunction4_f);

        // Delay: '<S436>/Delay1'
        if (Parker_Velocity_Control_DW.icLoad != 0) {
          Parker_Velocity_Control_DW.Delay1_DSTATE =
            Parker_Velocity_Control_B.sf_MATLABFunction4_f.e_out;
        }

        // Sum: '<S436>/Sum6' incorporates:
        //   Delay: '<S436>/Delay1'

        Parker_Velocity_Control_B.Subtract_o =
          Parker_Velocity_Control_B.sf_MATLABFunction4_f.e_out -
          Parker_Velocity_Control_DW.Delay1_DSTATE;

        // If: '<S436>/if we went through a "step"' incorporates:
        //   Inport: '<S437>/In1'

        if (Parker_Velocity_Control_B.Subtract_o != 0.0) {
          // Outputs for IfAction SubSystem: '<S436>/Hold this value' incorporates:
          //   ActionPort: '<S437>/Action Port'

          Parker_Velocity_Control_B.In1_i = Parker_Velocity_Control_B.Subtract_o;

          // End of Outputs for SubSystem: '<S436>/Hold this value'
        }

        // End of If: '<S436>/if we went through a "step"'

        // Sum: '<S430>/Sum3' incorporates:
        //   DataStoreWrite: '<S402>/RED_Tz'
        //   Gain: '<S430>/kd_tr'
        //   Gain: '<S430>/kp_tr'
        //   Gain: '<S436>/divide by delta T'

        Parker_Velocity_Control_DW.RED_Tz = 1.0 /
          Parker_Velocity_Control_P.baseRate * Parker_Velocity_Control_B.In1_i *
          Parker_Velocity_Control_P.Kd_tr + Parker_Velocity_Control_P.Kp_tr *
          Parker_Velocity_Control_B.sf_MATLABFunction4_f.e_out;

        // Sum: '<S402>/Subtract' incorporates:
        //   Constant: '<S402>/Desired Px (BLACK)'
        //   DataStoreRead: '<S402>/RED_Px'

        Parker_Velocity_Control_B.Subtract_o =
          Parker_Velocity_Control_P.home_states_RED[0] -
          Parker_Velocity_Control_DW.RED_Px;

        // Delay: '<S438>/Delay1'
        if (Parker_Velocity_Control_DW.icLoad_l != 0) {
          Parker_Velocity_Control_DW.Delay1_DSTATE_d =
            Parker_Velocity_Control_B.Subtract_o;
        }

        // Sum: '<S438>/Sum6' incorporates:
        //   Delay: '<S438>/Delay1'

        Parker_Velocity_Control_B.Sum6_l = Parker_Velocity_Control_B.Subtract_o
          - Parker_Velocity_Control_DW.Delay1_DSTATE_d;

        // If: '<S438>/if we went through a "step"' incorporates:
        //   Inport: '<S439>/In1'

        if (Parker_Velocity_Control_B.Sum6_l != 0.0) {
          // Outputs for IfAction SubSystem: '<S438>/Hold this value' incorporates:
          //   ActionPort: '<S439>/Action Port'

          Parker_Velocity_Control_B.In1_e = Parker_Velocity_Control_B.Sum6_l;

          // End of Outputs for SubSystem: '<S438>/Hold this value'
        }

        // End of If: '<S438>/if we went through a "step"'

        // Sum: '<S431>/Sum3' incorporates:
        //   DataStoreWrite: '<S402>/RED_Fx'
        //   Gain: '<S431>/kd_xr'
        //   Gain: '<S431>/kp_xr'
        //   Gain: '<S438>/divide by delta T'

        Parker_Velocity_Control_DW.RED_Fx = 1.0 /
          Parker_Velocity_Control_P.baseRate * Parker_Velocity_Control_B.In1_e *
          Parker_Velocity_Control_P.Kd_xr + Parker_Velocity_Control_P.Kp_xr *
          Parker_Velocity_Control_B.Subtract_o;

        // Sum: '<S402>/Subtract1' incorporates:
        //   Constant: '<S402>/Desired Px (BLACK)1'
        //   DataStoreRead: '<S402>/RED_Py '

        Parker_Velocity_Control_B.Sum6_l =
          Parker_Velocity_Control_P.home_states_RED[1] -
          Parker_Velocity_Control_DW.RED_Py;

        // Delay: '<S440>/Delay1'
        if (Parker_Velocity_Control_DW.icLoad_j != 0) {
          Parker_Velocity_Control_DW.Delay1_DSTATE_c =
            Parker_Velocity_Control_B.Sum6_l;
        }

        // Sum: '<S440>/Sum6' incorporates:
        //   Delay: '<S440>/Delay1'

        Parker_Velocity_Control_B.Sum6_d = Parker_Velocity_Control_B.Sum6_l -
          Parker_Velocity_Control_DW.Delay1_DSTATE_c;

        // If: '<S440>/if we went through a "step"' incorporates:
        //   Inport: '<S441>/In1'

        if (Parker_Velocity_Control_B.Sum6_d != 0.0) {
          // Outputs for IfAction SubSystem: '<S440>/Hold this value' incorporates:
          //   ActionPort: '<S441>/Action Port'

          Parker_Velocity_Control_B.In1 = Parker_Velocity_Control_B.Sum6_d;

          // End of Outputs for SubSystem: '<S440>/Hold this value'
        }

        // End of If: '<S440>/if we went through a "step"'

        // Sum: '<S432>/Sum3' incorporates:
        //   DataStoreWrite: '<S402>/RED_Fy'
        //   Gain: '<S432>/kd_yr'
        //   Gain: '<S432>/kp_yr'
        //   Gain: '<S440>/divide by delta T'

        Parker_Velocity_Control_DW.RED_Fy = 1.0 /
          Parker_Velocity_Control_P.baseRate * Parker_Velocity_Control_B.In1 *
          Parker_Velocity_Control_P.Kd_yr + Parker_Velocity_Control_P.Kp_yr *
          Parker_Velocity_Control_B.Sum6_l;

        // DataStoreWrite: '<S402>/Data Store Write3' incorporates:
        //   Constant: '<S402>/Puck State'

        Parker_Velocity_Control_DW.Float_State =
          Parker_Velocity_Control_P.PuckState_Value_e;

        // If: '<S402>/If' incorporates:
        //   Constant: '<S402>/Constant3'

        if ((Parker_Velocity_Control_P.platformSelection == 4.0) ||
            (Parker_Velocity_Control_P.platformSelection == 5.0) ||
            (Parker_Velocity_Control_P.platformSelection == 10.0) ||
            (Parker_Velocity_Control_P.platformSelection == 11.0)) {
          // Outputs for IfAction SubSystem: '<S402>/RED+ARM' incorporates:
          //   ActionPort: '<S435>/Action Port'

          Parker_Velocity_Cont_REDARM(&Parker_Velocity_Control_B.REDARM_bo,
            &Parker_Velocity_Control_P.REDARM_bo,
            &Parker_Velocity_Control_DW.RED_RzD_Elbow,
            &Parker_Velocity_Control_DW.RED_RzD_Shoulder,
            &Parker_Velocity_Control_DW.RED_RzD_Wrist,
            &Parker_Velocity_Control_DW.RED_Rz_Elbow,
            &Parker_Velocity_Control_DW.RED_Rz_Shoulder,
            &Parker_Velocity_Control_DW.RED_Rz_Wrist,
            &Parker_Velocity_Control_DW.RED_Tz_Elbow,
            &Parker_Velocity_Control_DW.RED_Tz_Shoulder,
            &Parker_Velocity_Control_DW.RED_Tz_Wrist);

          // End of Outputs for SubSystem: '<S402>/RED+ARM'
        } else {
          // Outputs for IfAction SubSystem: '<S402>/RED' incorporates:
          //   ActionPort: '<S434>/Action Port'

          // DataStoreWrite: '<S434>/RED_Fx1' incorporates:
          //   Constant: '<S434>/Constant7'

          Parker_Velocity_Control_DW.RED_Tz_Shoulder =
            Parker_Velocity_Control_P.Constant7_Value_a;

          // DataStoreWrite: '<S434>/RED_Fy1' incorporates:
          //   Constant: '<S434>/Constant7'

          Parker_Velocity_Control_DW.RED_Tz_Elbow =
            Parker_Velocity_Control_P.Constant7_Value_a;

          // DataStoreWrite: '<S434>/RED_Tz1' incorporates:
          //   Constant: '<S434>/Constant7'

          Parker_Velocity_Control_DW.RED_Tz_Wrist =
            Parker_Velocity_Control_P.Constant7_Value_a;

          // End of Outputs for SubSystem: '<S402>/RED'
        }

        // End of If: '<S402>/If'

        // Update for Delay: '<S436>/Delay1'
        Parker_Velocity_Control_DW.icLoad = 0U;
        Parker_Velocity_Control_DW.Delay1_DSTATE =
          Parker_Velocity_Control_B.sf_MATLABFunction4_f.e_out;

        // Update for Delay: '<S438>/Delay1'
        Parker_Velocity_Control_DW.icLoad_l = 0U;
        Parker_Velocity_Control_DW.Delay1_DSTATE_d =
          Parker_Velocity_Control_B.Subtract_o;

        // Update for Delay: '<S440>/Delay1'
        Parker_Velocity_Control_DW.icLoad_j = 0U;
        Parker_Velocity_Control_DW.Delay1_DSTATE_c =
          Parker_Velocity_Control_B.Sum6_l;

        // End of Outputs for SubSystem: '<S15>/Change RED Behavior'
      }

      // End of If: '<S15>/This IF block determines whether or not to run the RED sim//exp ' 
      // End of Outputs for SubSystem: '<Root>/Phase #5:  Hold Home'
    } else {
      // Outputs for IfAction SubSystem: '<Root>/Phase #6:  Stop Floating and Spin Down RW' incorporates:
      //   ActionPort: '<S16>/Action Port'

      if ((Parker_Velocity_Control_DW.WhoAmI == 2.0) ||
          (Parker_Velocity_Control_P.simMode == 1.0)) {
        // Outputs for IfAction SubSystem: '<S16>/Change BLACK Behavior' incorporates:
        //   ActionPort: '<S453>/Action Port'

        // If: '<S16>/This IF block determines whether or not to run the BLACK sim//exp' incorporates:
        //   Constant: '<S453>/Constant3'
        //   Constant: '<S453>/Constant4'
        //   Constant: '<S453>/Constant5'
        //   Constant: '<S453>/Puck State'
        //   DataStoreWrite: '<S453>/BLACK_Fx'
        //   DataStoreWrite: '<S453>/BLACK_Fy'
        //   DataStoreWrite: '<S453>/BLACK_Tz'
        //   DataStoreWrite: '<S453>/Data Store Write3'

        Parker_Velocity_Control_DW.BLACK_Fx =
          Parker_Velocity_Control_P.Constant3_Value_d;
        Parker_Velocity_Control_DW.BLACK_Fy =
          Parker_Velocity_Control_P.Constant4_Value_e;
        Parker_Velocity_Control_DW.BLACK_Tz =
          Parker_Velocity_Control_P.Constant5_Value;
        Parker_Velocity_Control_DW.Float_State =
          Parker_Velocity_Control_P.PuckState_Value_gd;

        // End of Outputs for SubSystem: '<S16>/Change BLACK Behavior'
      }

      // If: '<S16>/This IF block determines whether or not to run the BLUE sim//exp' incorporates:
      //   Constant: '<S456>/Constant'
      //   DataStoreRead: '<S16>/Data Store Read'

      if ((Parker_Velocity_Control_DW.WhoAmI == 3.0) ||
          (Parker_Velocity_Control_P.simMode == 1.0)) {
        // Outputs for IfAction SubSystem: '<S16>/Change BLUE Behavior' incorporates:
        //   ActionPort: '<S454>/Action Port'

        Parker_V_ChangeBLUEBehavior
          (&Parker_Velocity_Control_P.ChangeBLUEBehavior_k,
           &Parker_Velocity_Control_DW.BLUE_Fx,
           &Parker_Velocity_Control_DW.BLUE_Fy,
           &Parker_Velocity_Control_DW.BLUE_Tz,
           &Parker_Velocity_Control_DW.Float_State);

        // End of Outputs for SubSystem: '<S16>/Change BLUE Behavior'
      }

      // End of If: '<S16>/This IF block determines whether or not to run the BLUE sim//exp' 

      // If: '<S16>/This IF block determines whether or not to run the RED sim//exp ' incorporates:
      //   Constant: '<S456>/Constant'
      //   DataStoreRead: '<S16>/Data Store Read'

      if ((Parker_Velocity_Control_DW.WhoAmI == 1.0) ||
          (Parker_Velocity_Control_P.simMode == 1.0)) {
        // Outputs for IfAction SubSystem: '<S16>/Change RED Behavior' incorporates:
        //   ActionPort: '<S455>/Action Port'

        // DataStoreWrite: '<S455>/RED_Fx' incorporates:
        //   Constant: '<S455>/Constant'

        Parker_Velocity_Control_DW.RED_Fx =
          Parker_Velocity_Control_P.Constant_Value_j;

        // DataStoreWrite: '<S455>/RED_Fy' incorporates:
        //   Constant: '<S455>/Constant1'

        Parker_Velocity_Control_DW.RED_Fy =
          Parker_Velocity_Control_P.Constant1_Value_l;

        // DataStoreWrite: '<S455>/RED_Tz' incorporates:
        //   Constant: '<S455>/Constant2'

        Parker_Velocity_Control_DW.RED_Tz =
          Parker_Velocity_Control_P.Constant2_Value_m;

        // DataStoreWrite: '<S455>/Data Store Write' incorporates:
        //   Constant: '<S455>/Constant4'

        Parker_Velocity_Control_DW.RED_Tz_Shoulder =
          Parker_Velocity_Control_P.Constant4_Value_o;

        // DataStoreWrite: '<S455>/Data Store Write1' incorporates:
        //   Constant: '<S455>/Constant5'

        Parker_Velocity_Control_DW.RED_Tz_Elbow =
          Parker_Velocity_Control_P.Constant5_Value_f;

        // DataStoreWrite: '<S455>/Data Store Write2' incorporates:
        //   Constant: '<S455>/Constant6'

        Parker_Velocity_Control_DW.RED_Tz_Wrist =
          Parker_Velocity_Control_P.Constant6_Value;

        // DataStoreWrite: '<S455>/Data Store Write3' incorporates:
        //   Constant: '<S455>/Puck State'

        Parker_Velocity_Control_DW.Float_State =
          Parker_Velocity_Control_P.PuckState_Value_h;

        // Gain: '<S455>/Gain' incorporates:
        //   DataStoreRead: '<S455>/RED_dRz_Speed'

        Parker_Velocity_Control_B.Subtract_o =
          Parker_Velocity_Control_P.Gain_Gain_a *
          Parker_Velocity_Control_DW.RED_dRz_RW_Sat;

        // Saturate: '<S455>/Saturation'
        if (Parker_Velocity_Control_B.Subtract_o >
            Parker_Velocity_Control_P.Saturation_UpperSat) {
          Parker_Velocity_Control_DW.RED_Tz_RW =
            Parker_Velocity_Control_P.Saturation_UpperSat;
        } else if (Parker_Velocity_Control_B.Subtract_o <
                   Parker_Velocity_Control_P.Saturation_LowerSat) {
          Parker_Velocity_Control_DW.RED_Tz_RW =
            Parker_Velocity_Control_P.Saturation_LowerSat;
        } else {
          Parker_Velocity_Control_DW.RED_Tz_RW =
            Parker_Velocity_Control_B.Subtract_o;
        }

        // End of Saturate: '<S455>/Saturation'
        // End of Outputs for SubSystem: '<S16>/Change RED Behavior'
      }

      // End of If: '<S16>/This IF block determines whether or not to run the RED sim//exp ' 
      // End of Outputs for SubSystem: '<Root>/Phase #6:  Stop Floating and Spin Down RW' 
    }

    // End of If: '<Root>/Separate Phases'

    // If: '<Root>/If running a simulation,  grab the simulated states.' incorporates:
    //   Constant: '<S7>/Constant'
    //   DiscreteIntegrator: '<S461>/Acceleration  to Velocity'
    //   DiscreteIntegrator: '<S461>/Velocity to Position'
    //   DiscreteIntegrator: '<S473>/Acceleration  to Velocity'
    //   DiscreteIntegrator: '<S473>/Velocity to Position'

    if (Parker_Velocity_Control_P.simMode == 1.0) {
      // Outputs for IfAction SubSystem: '<Root>/Simulate Plant Dynamics' incorporates:
      //   ActionPort: '<S19>/Action Port'

      // DiscreteIntegrator: '<S461>/Acceleration  to Velocity' incorporates:
      //   DataStoreRead: '<S457>/BLACK_Fx_Sat'
      //   MATLAB Function: '<S461>/MATLAB Function'
      //   SignalConversion generated from: '<S467>/ SFunction '

      // MATLAB Function 'Simulate Plant Dynamics/BLACK Dynamics/BLACK Dynamics Model/MATLAB Function': '<S467>:1' 
      // '<S467>:1:5' x_ddot     = zeros(3,1);
      // '<S467>:1:9' m_BLACK    = model_param(3);
      // '<S467>:1:10' I_BLACK    = model_param(4);
      // '<S467>:1:13' Fx        = control_inputs(1);
      // '<S467>:1:14' Fy        = control_inputs(2);
      // '<S467>:1:15' Tz        = control_inputs(3);
      // '<S467>:1:18' x_ddot(1) = Fx/m_BLACK;
      // '<S467>:1:19' x_ddot(2) = Fy/m_BLACK;
      // '<S467>:1:20' x_ddot(3) = Tz/I_BLACK;
      Parker_Velocity_Control_B.rtb_x_ddot_b_g =
        Parker_Velocity_Control_DW.BLACK_Fx_Sat /
        Parker_Velocity_Control_P.model_param[2] *
        Parker_Velocity_Control_P.AccelerationtoVelocity_gainva_i +
        Parker_Velocity_Control_DW.AccelerationtoVelocity_DSTATE[0];

      // DiscreteIntegrator: '<S461>/Velocity to Position'
      Parker_Velocity_Control_B.VelocitytoPosition =
        Parker_Velocity_Control_P.VelocitytoPosition_gainval_c *
        Parker_Velocity_Control_B.rtb_x_ddot_b_g +
        Parker_Velocity_Control_DW.VelocitytoPosition_DSTATE[0];

      // Sum: '<S461>/Sum' incorporates:
      //   RandomNumber: '<S461>/Random Number'

      Parker_Velocity_Control_B.rtb_Sum_idx_0 =
        Parker_Velocity_Control_B.VelocitytoPosition +
        Parker_Velocity_Control_DW.NextOutput;
      Parker_Velocity_Control_B.x_ddot_b[0] =
        Parker_Velocity_Control_B.rtb_x_ddot_b_g;
      Parker_Velocity_Control_B.VelocitytoPosition_idx_0 =
        Parker_Velocity_Control_B.VelocitytoPosition;

      // DiscreteIntegrator: '<S461>/Acceleration  to Velocity' incorporates:
      //   DataStoreRead: '<S457>/BLACK_Fy_Sat'
      //   DiscreteIntegrator: '<S461>/Velocity to Position'
      //   MATLAB Function: '<S461>/MATLAB Function'
      //   SignalConversion generated from: '<S467>/ SFunction '

      Parker_Velocity_Control_B.rtb_x_ddot_b_g =
        Parker_Velocity_Control_DW.BLACK_Fy_Sat /
        Parker_Velocity_Control_P.model_param[2] *
        Parker_Velocity_Control_P.AccelerationtoVelocity_gainva_i +
        Parker_Velocity_Control_DW.AccelerationtoVelocity_DSTATE[1];

      // DiscreteIntegrator: '<S461>/Velocity to Position'
      Parker_Velocity_Control_B.VelocitytoPosition =
        Parker_Velocity_Control_P.VelocitytoPosition_gainval_c *
        Parker_Velocity_Control_B.rtb_x_ddot_b_g +
        Parker_Velocity_Control_DW.VelocitytoPosition_DSTATE[1];

      // Sum: '<S461>/Sum' incorporates:
      //   RandomNumber: '<S461>/Random Number'

      Parker_Velocity_Control_B.rtb_Sum_idx_1 =
        Parker_Velocity_Control_B.VelocitytoPosition +
        Parker_Velocity_Control_DW.NextOutput;
      Parker_Velocity_Control_B.x_ddot_b[1] =
        Parker_Velocity_Control_B.rtb_x_ddot_b_g;
      Parker_Velocity_Control_B.VelocitytoPosition_idx_1 =
        Parker_Velocity_Control_B.VelocitytoPosition;

      // DiscreteIntegrator: '<S461>/Acceleration  to Velocity' incorporates:
      //   DataStoreRead: '<S457>/BLACK_Tz_Sat'
      //   DiscreteIntegrator: '<S461>/Velocity to Position'
      //   MATLAB Function: '<S461>/MATLAB Function'
      //   SignalConversion generated from: '<S467>/ SFunction '

      Parker_Velocity_Control_B.rtb_x_ddot_b_g =
        Parker_Velocity_Control_DW.BLACK_Tz_Sat /
        Parker_Velocity_Control_P.model_param[3] *
        Parker_Velocity_Control_P.AccelerationtoVelocity_gainva_i +
        Parker_Velocity_Control_DW.AccelerationtoVelocity_DSTATE[2];

      // DiscreteIntegrator: '<S461>/Velocity to Position'
      Parker_Velocity_Control_B.VelocitytoPosition =
        Parker_Velocity_Control_P.VelocitytoPosition_gainval_c *
        Parker_Velocity_Control_B.rtb_x_ddot_b_g +
        Parker_Velocity_Control_DW.VelocitytoPosition_DSTATE[2];

      // Sum: '<S461>/Sum' incorporates:
      //   RandomNumber: '<S461>/Random Number'

      Parker_Velocity_Control_B.rtb_Sum_idx_2 =
        Parker_Velocity_Control_B.VelocitytoPosition +
        Parker_Velocity_Control_DW.NextOutput;

      // Delay: '<S466>/Delay1'
      if (Parker_Velocity_Control_DW.icLoad_bb != 0) {
        Parker_Velocity_Control_DW.Delay1_DSTATE_p =
          Parker_Velocity_Control_B.rtb_Sum_idx_0;
      }

      // Sum: '<S466>/Sum6' incorporates:
      //   Delay: '<S466>/Delay1'

      Parker_Velocity_Control_B.Subtract_o =
        Parker_Velocity_Control_B.rtb_Sum_idx_0 -
        Parker_Velocity_Control_DW.Delay1_DSTATE_p;

      // If: '<S466>/if we went through a "step"' incorporates:
      //   Inport: '<S472>/In1'

      if (Parker_Velocity_Control_B.Subtract_o != 0.0) {
        // Outputs for IfAction SubSystem: '<S466>/Hold this value' incorporates:
        //   ActionPort: '<S472>/Action Port'

        Parker_Velocity_Control_B.In1_es = Parker_Velocity_Control_B.Subtract_o;

        // End of Outputs for SubSystem: '<S466>/Hold this value'
      }

      // End of If: '<S466>/if we went through a "step"'

      // Gain: '<S466>/divide by delta T' incorporates:
      //   DataStoreWrite: '<S457>/BLACK_Vx'

      Parker_Velocity_Control_DW.BLACK_Vx = 1.0 /
        Parker_Velocity_Control_P.baseRate * Parker_Velocity_Control_B.In1_es;

      // Delay: '<S464>/Delay1' incorporates:
      //   DataStoreWrite: '<S457>/BLACK_Vx'

      if (Parker_Velocity_Control_DW.icLoad_p != 0) {
        Parker_Velocity_Control_DW.Delay1_DSTATE_ja =
          Parker_Velocity_Control_DW.BLACK_Vx;
      }

      // Sum: '<S464>/Sum6' incorporates:
      //   DataStoreWrite: '<S457>/BLACK_Vx'
      //   Delay: '<S464>/Delay1'

      Parker_Velocity_Control_B.Subtract_o = Parker_Velocity_Control_DW.BLACK_Vx
        - Parker_Velocity_Control_DW.Delay1_DSTATE_ja;

      // If: '<S464>/if we went through a "step"' incorporates:
      //   Inport: '<S470>/In1'

      if (Parker_Velocity_Control_B.Subtract_o != 0.0) {
        // Outputs for IfAction SubSystem: '<S464>/Hold this value' incorporates:
        //   ActionPort: '<S470>/Action Port'

        Parker_Velocity_Control_B.In1_kz = Parker_Velocity_Control_B.Subtract_o;

        // End of Outputs for SubSystem: '<S464>/Hold this value'
      }

      // End of If: '<S464>/if we went through a "step"'

      // Sum: '<S457>/Sum1' incorporates:
      //   DataStoreWrite: '<S457>/BLACK_IMU_Ax_I'
      //   Gain: '<S464>/divide by delta T'
      //   RandomNumber: '<S457>/Random Number1'

      Parker_Velocity_Control_DW.BLACK_IMU_Ax_I = 1.0 /
        Parker_Velocity_Control_P.baseRate * Parker_Velocity_Control_B.In1_kz +
        Parker_Velocity_Control_DW.NextOutput_p;

      // Delay: '<S462>/Delay1'
      if (Parker_Velocity_Control_DW.icLoad_lv != 0) {
        Parker_Velocity_Control_DW.Delay1_DSTATE_d4 =
          Parker_Velocity_Control_B.rtb_Sum_idx_1;
      }

      // Sum: '<S462>/Sum6' incorporates:
      //   Delay: '<S462>/Delay1'

      Parker_Velocity_Control_B.Subtract_o =
        Parker_Velocity_Control_B.rtb_Sum_idx_1 -
        Parker_Velocity_Control_DW.Delay1_DSTATE_d4;

      // If: '<S462>/if we went through a "step"' incorporates:
      //   Inport: '<S468>/In1'

      if (Parker_Velocity_Control_B.Subtract_o != 0.0) {
        // Outputs for IfAction SubSystem: '<S462>/Hold this value' incorporates:
        //   ActionPort: '<S468>/Action Port'

        Parker_Velocity_Control_B.In1_aw = Parker_Velocity_Control_B.Subtract_o;

        // End of Outputs for SubSystem: '<S462>/Hold this value'
      }

      // End of If: '<S462>/if we went through a "step"'

      // Gain: '<S462>/divide by delta T' incorporates:
      //   DataStoreWrite: '<S457>/BLACK_Vy'

      Parker_Velocity_Control_DW.BLACK_Vy = 1.0 /
        Parker_Velocity_Control_P.baseRate * Parker_Velocity_Control_B.In1_aw;

      // Delay: '<S465>/Delay1' incorporates:
      //   DataStoreWrite: '<S457>/BLACK_Vy'

      if (Parker_Velocity_Control_DW.icLoad_hu != 0) {
        Parker_Velocity_Control_DW.Delay1_DSTATE_j3 =
          Parker_Velocity_Control_DW.BLACK_Vy;
      }

      // Sum: '<S465>/Sum6' incorporates:
      //   DataStoreWrite: '<S457>/BLACK_Vy'
      //   Delay: '<S465>/Delay1'

      Parker_Velocity_Control_B.Subtract_o = Parker_Velocity_Control_DW.BLACK_Vy
        - Parker_Velocity_Control_DW.Delay1_DSTATE_j3;

      // If: '<S465>/if we went through a "step"' incorporates:
      //   Inport: '<S471>/In1'

      if (Parker_Velocity_Control_B.Subtract_o != 0.0) {
        // Outputs for IfAction SubSystem: '<S465>/Hold this value' incorporates:
        //   ActionPort: '<S471>/Action Port'

        Parker_Velocity_Control_B.In1_id = Parker_Velocity_Control_B.Subtract_o;

        // End of Outputs for SubSystem: '<S465>/Hold this value'
      }

      // End of If: '<S465>/if we went through a "step"'

      // Sum: '<S457>/Sum2' incorporates:
      //   DataStoreWrite: '<S457>/BLACK_IMU_Ay_I'
      //   Gain: '<S465>/divide by delta T'
      //   RandomNumber: '<S457>/Random Number'

      Parker_Velocity_Control_DW.BLACK_IMU_Ay_I = 1.0 /
        Parker_Velocity_Control_P.baseRate * Parker_Velocity_Control_B.In1_id +
        Parker_Velocity_Control_DW.NextOutput_o;

      // Delay: '<S463>/Delay1'
      if (Parker_Velocity_Control_DW.icLoad_m != 0) {
        Parker_Velocity_Control_DW.Delay1_DSTATE_km =
          Parker_Velocity_Control_B.rtb_Sum_idx_2;
      }

      // Sum: '<S463>/Sum6' incorporates:
      //   Delay: '<S463>/Delay1'

      Parker_Velocity_Control_B.Subtract_o =
        Parker_Velocity_Control_B.rtb_Sum_idx_2 -
        Parker_Velocity_Control_DW.Delay1_DSTATE_km;

      // If: '<S463>/if we went through a "step"' incorporates:
      //   Inport: '<S469>/In1'

      if (Parker_Velocity_Control_B.Subtract_o != 0.0) {
        // Outputs for IfAction SubSystem: '<S463>/Hold this value' incorporates:
        //   ActionPort: '<S469>/Action Port'

        Parker_Velocity_Control_B.In1_b2 = Parker_Velocity_Control_B.Subtract_o;

        // End of Outputs for SubSystem: '<S463>/Hold this value'
      }

      // End of If: '<S463>/if we went through a "step"'

      // Gain: '<S463>/divide by delta T' incorporates:
      //   DataStoreWrite: '<S457>/BLACK_RzD'

      Parker_Velocity_Control_DW.BLACK_RzD = 1.0 /
        Parker_Velocity_Control_P.baseRate * Parker_Velocity_Control_B.In1_b2;

      // Sum: '<S457>/Sum3' incorporates:
      //   DataStoreWrite: '<S457>/BLACK_IMU_R'
      //   DataStoreWrite: '<S457>/BLACK_RzD'
      //   RandomNumber: '<S457>/Random Number2'

      Parker_Velocity_Control_DW.BLACK_IMU_R =
        Parker_Velocity_Control_DW.BLACK_RzD +
        Parker_Velocity_Control_DW.NextOutput_j;

      // DataStoreWrite: '<S457>/BLACK_Px'
      Parker_Velocity_Control_DW.BLACK_Px =
        Parker_Velocity_Control_B.rtb_Sum_idx_0;

      // DataStoreWrite: '<S457>/BLACK_Py'
      Parker_Velocity_Control_DW.BLACK_Py =
        Parker_Velocity_Control_B.rtb_Sum_idx_1;

      // DataStoreWrite: '<S457>/BLACK_Rz'
      Parker_Velocity_Control_DW.BLACK_Rz =
        Parker_Velocity_Control_B.rtb_Sum_idx_2;

      // DiscreteIntegrator: '<S473>/Acceleration  to Velocity' incorporates:
      //   DataStoreRead: '<S458>/BLUE_Fx_Sat '
      //   MATLAB Function: '<S473>/MATLAB Function'
      //   SignalConversion generated from: '<S479>/ SFunction '

      // MATLAB Function 'Simulate Plant Dynamics/BLUE Dynamics/BLUE  Dynamics Model/MATLAB Function': '<S479>:1' 
      // '<S479>:1:5' x_ddot     = zeros(3,1);
      // '<S479>:1:9' m_BLUE    = model_param(5);
      // '<S479>:1:10' I_BLUE    = model_param(6);
      // '<S479>:1:13' Fx        = control_inputs(1);
      // '<S479>:1:14' Fy        = control_inputs(2);
      // '<S479>:1:15' Tz        = control_inputs(3);
      // '<S479>:1:18' x_ddot(1) = Fx/m_BLUE;
      // '<S479>:1:19' x_ddot(2) = Fy/m_BLUE;
      // '<S479>:1:20' x_ddot(3) = Tz/I_BLUE;
      Parker_Velocity_Control_B.rtb_x_ddot_i_m =
        Parker_Velocity_Control_DW.BLUE_Fx_Sat /
        Parker_Velocity_Control_P.model_param[4] *
        Parker_Velocity_Control_P.AccelerationtoVelocity_gainva_b +
        Parker_Velocity_Control_DW.AccelerationtoVelocity_DSTATE_b[0];

      // DiscreteIntegrator: '<S473>/Velocity to Position'
      Parker_Velocity_Control_B.rtb_TmpSignalConversionAtSFu_nz =
        Parker_Velocity_Control_P.VelocitytoPosition_gainval_f *
        Parker_Velocity_Control_B.rtb_x_ddot_i_m +
        Parker_Velocity_Control_DW.VelocitytoPosition_DSTATE_a[0];

      // Sum: '<S473>/Sum' incorporates:
      //   RandomNumber: '<S473>/Random Number'

      Parker_Velocity_Control_B.rtb_Sum_l_idx_0 =
        Parker_Velocity_Control_B.rtb_TmpSignalConversionAtSFu_nz +
        Parker_Velocity_Control_DW.NextOutput_g;
      Parker_Velocity_Control_B.rtb_x_ddot_i_idx_0 =
        Parker_Velocity_Control_B.rtb_x_ddot_i_m;
      Parker_Velocity_Control_B.rtb_TmpSignalConversionAtSFun_p =
        Parker_Velocity_Control_B.rtb_TmpSignalConversionAtSFu_nz;

      // DiscreteIntegrator: '<S473>/Acceleration  to Velocity' incorporates:
      //   DataStoreRead: '<S458>/BLUE_Fy_Sat'
      //   DiscreteIntegrator: '<S473>/Velocity to Position'
      //   MATLAB Function: '<S473>/MATLAB Function'
      //   SignalConversion generated from: '<S479>/ SFunction '

      Parker_Velocity_Control_B.rtb_x_ddot_i_m =
        Parker_Velocity_Control_DW.BLUE_Fy_Sat /
        Parker_Velocity_Control_P.model_param[4] *
        Parker_Velocity_Control_P.AccelerationtoVelocity_gainva_b +
        Parker_Velocity_Control_DW.AccelerationtoVelocity_DSTATE_b[1];

      // DiscreteIntegrator: '<S473>/Velocity to Position'
      Parker_Velocity_Control_B.rtb_TmpSignalConversionAtSFu_nz =
        Parker_Velocity_Control_P.VelocitytoPosition_gainval_f *
        Parker_Velocity_Control_B.rtb_x_ddot_i_m +
        Parker_Velocity_Control_DW.VelocitytoPosition_DSTATE_a[1];

      // Sum: '<S473>/Sum' incorporates:
      //   RandomNumber: '<S473>/Random Number'

      Parker_Velocity_Control_B.rtb_Sum_l_idx_1 =
        Parker_Velocity_Control_B.rtb_TmpSignalConversionAtSFu_nz +
        Parker_Velocity_Control_DW.NextOutput_g;
      Parker_Velocity_Control_B.rtb_x_ddot_i_idx_1 =
        Parker_Velocity_Control_B.rtb_x_ddot_i_m;
      Parker_Velocity_Control_B.rtb_TmpSignalConversionAtSFun_l =
        Parker_Velocity_Control_B.rtb_TmpSignalConversionAtSFu_nz;

      // DiscreteIntegrator: '<S473>/Acceleration  to Velocity' incorporates:
      //   DataStoreRead: '<S458>/BLUE_Tz_Sat'
      //   DiscreteIntegrator: '<S473>/Velocity to Position'
      //   MATLAB Function: '<S473>/MATLAB Function'
      //   SignalConversion generated from: '<S479>/ SFunction '

      Parker_Velocity_Control_B.rtb_x_ddot_i_m =
        Parker_Velocity_Control_DW.BLUE_Tz_Sat /
        Parker_Velocity_Control_P.model_param[5] *
        Parker_Velocity_Control_P.AccelerationtoVelocity_gainva_b +
        Parker_Velocity_Control_DW.AccelerationtoVelocity_DSTATE_b[2];

      // DiscreteIntegrator: '<S473>/Velocity to Position'
      Parker_Velocity_Control_B.rtb_TmpSignalConversionAtSFu_nz =
        Parker_Velocity_Control_P.VelocitytoPosition_gainval_f *
        Parker_Velocity_Control_B.rtb_x_ddot_i_m +
        Parker_Velocity_Control_DW.VelocitytoPosition_DSTATE_a[2];

      // Sum: '<S473>/Sum' incorporates:
      //   RandomNumber: '<S473>/Random Number'

      Parker_Velocity_Control_B.rtb_Sum_l_idx_2 =
        Parker_Velocity_Control_B.rtb_TmpSignalConversionAtSFu_nz +
        Parker_Velocity_Control_DW.NextOutput_g;

      // Delay: '<S478>/Delay1'
      if (Parker_Velocity_Control_DW.icLoad_dc != 0) {
        Parker_Velocity_Control_DW.Delay1_DSTATE_e =
          Parker_Velocity_Control_B.rtb_Sum_l_idx_0;
      }

      // Sum: '<S478>/Sum6' incorporates:
      //   Delay: '<S478>/Delay1'

      Parker_Velocity_Control_B.Subtract_o =
        Parker_Velocity_Control_B.rtb_Sum_l_idx_0 -
        Parker_Velocity_Control_DW.Delay1_DSTATE_e;

      // If: '<S478>/if we went through a "step"' incorporates:
      //   Inport: '<S484>/In1'

      if (Parker_Velocity_Control_B.Subtract_o != 0.0) {
        // Outputs for IfAction SubSystem: '<S478>/Hold this value' incorporates:
        //   ActionPort: '<S484>/Action Port'

        Parker_Velocity_Control_B.In1_adc = Parker_Velocity_Control_B.Subtract_o;

        // End of Outputs for SubSystem: '<S478>/Hold this value'
      }

      // End of If: '<S478>/if we went through a "step"'

      // Gain: '<S478>/divide by delta T' incorporates:
      //   DataStoreWrite: '<S458>/BLUE_Vx'

      Parker_Velocity_Control_DW.BLUE_Vx = 1.0 /
        Parker_Velocity_Control_P.baseRate * Parker_Velocity_Control_B.In1_adc;

      // Delay: '<S476>/Delay1' incorporates:
      //   DataStoreWrite: '<S458>/BLUE_Vx'

      if (Parker_Velocity_Control_DW.icLoad_ln != 0) {
        Parker_Velocity_Control_DW.Delay1_DSTATE_fi =
          Parker_Velocity_Control_DW.BLUE_Vx;
      }

      // End of Delay: '<S476>/Delay1'

      // Delay: '<S474>/Delay1'
      if (Parker_Velocity_Control_DW.icLoad_oc != 0) {
        Parker_Velocity_Control_DW.Delay1_DSTATE_b =
          Parker_Velocity_Control_B.rtb_Sum_l_idx_1;
      }

      // Sum: '<S474>/Sum6' incorporates:
      //   Delay: '<S474>/Delay1'

      Parker_Velocity_Control_B.Subtract_o =
        Parker_Velocity_Control_B.rtb_Sum_l_idx_1 -
        Parker_Velocity_Control_DW.Delay1_DSTATE_b;

      // If: '<S474>/if we went through a "step"' incorporates:
      //   Inport: '<S480>/In1'

      if (Parker_Velocity_Control_B.Subtract_o != 0.0) {
        // Outputs for IfAction SubSystem: '<S474>/Hold this value' incorporates:
        //   ActionPort: '<S480>/Action Port'

        Parker_Velocity_Control_B.In1_pr = Parker_Velocity_Control_B.Subtract_o;

        // End of Outputs for SubSystem: '<S474>/Hold this value'
      }

      // End of If: '<S474>/if we went through a "step"'

      // Gain: '<S474>/divide by delta T' incorporates:
      //   DataStoreWrite: '<S458>/BLUE_Vy'

      Parker_Velocity_Control_DW.BLUE_Vy = 1.0 /
        Parker_Velocity_Control_P.baseRate * Parker_Velocity_Control_B.In1_pr;

      // Delay: '<S477>/Delay1' incorporates:
      //   DataStoreWrite: '<S458>/BLUE_Vy'

      if (Parker_Velocity_Control_DW.icLoad_l1 != 0) {
        Parker_Velocity_Control_DW.Delay1_DSTATE_fq =
          Parker_Velocity_Control_DW.BLUE_Vy;
      }

      // End of Delay: '<S477>/Delay1'

      // Delay: '<S475>/Delay1'
      if (Parker_Velocity_Control_DW.icLoad_lz != 0) {
        Parker_Velocity_Control_DW.Delay1_DSTATE_od =
          Parker_Velocity_Control_B.rtb_Sum_l_idx_2;
      }

      // Sum: '<S475>/Sum6' incorporates:
      //   Delay: '<S475>/Delay1'

      Parker_Velocity_Control_B.Subtract_o =
        Parker_Velocity_Control_B.rtb_Sum_l_idx_2 -
        Parker_Velocity_Control_DW.Delay1_DSTATE_od;

      // If: '<S475>/if we went through a "step"' incorporates:
      //   Inport: '<S481>/In1'

      if (Parker_Velocity_Control_B.Subtract_o != 0.0) {
        // Outputs for IfAction SubSystem: '<S475>/Hold this value' incorporates:
        //   ActionPort: '<S481>/Action Port'

        Parker_Velocity_Control_B.In1_fv = Parker_Velocity_Control_B.Subtract_o;

        // End of Outputs for SubSystem: '<S475>/Hold this value'
      }

      // End of If: '<S475>/if we went through a "step"'

      // Gain: '<S475>/divide by delta T' incorporates:
      //   DataStoreWrite: '<S458>/BLUE_RzD'

      Parker_Velocity_Control_DW.BLUE_RzD = 1.0 /
        Parker_Velocity_Control_P.baseRate * Parker_Velocity_Control_B.In1_fv;

      // Sum: '<S458>/Sum6' incorporates:
      //   DataStoreWrite: '<S458>/BLUE_IMU_R'
      //   DataStoreWrite: '<S458>/BLUE_RzD'
      //   RandomNumber: '<S458>/Random Number5'

      Parker_Velocity_Control_B.Subtract_o = Parker_Velocity_Control_DW.BLUE_RzD
        + Parker_Velocity_Control_DW.NextOutput_k;

      // DataStoreWrite: '<S458>/BLUE_Px '
      Parker_Velocity_Control_DW.BLUE_Px =
        Parker_Velocity_Control_B.rtb_Sum_l_idx_0;

      // DataStoreWrite: '<S458>/BLUE_Py'
      Parker_Velocity_Control_DW.BLUE_Py =
        Parker_Velocity_Control_B.rtb_Sum_l_idx_1;

      // DataStoreWrite: '<S458>/BLUE_Rz'
      Parker_Velocity_Control_DW.BLUE_Rz =
        Parker_Velocity_Control_B.rtb_Sum_l_idx_2;

      // RelationalOperator: '<S485>/Compare' incorporates:
      //   Constant: '<S485>/Constant'
      //   DataStoreRead: '<S459>/Data Store Read1'

      Parker_Velocity_Control_B.rtb_Compare_j =
        (Parker_Velocity_Control_DW.BLUE_Rz ==
         Parker_Velocity_Control_P.Constant_Value);

      // DiscreteIntegrator: '<S459>/Discrete-Time Integrator' incorporates:
      //   DataStoreRead: '<S459>/Data Store Read1'

      if (Parker_Velocity_Control_DW.DiscreteTimeIntegrator_IC_LOADI != 0) {
        Parker_Velocity_Control_DW.DiscreteTimeIntegrator_DSTATE =
          Parker_Velocity_Control_DW.BLUE_Rz;
      }

      if ((Parker_Velocity_Control_B.rtb_Compare_j &&
           (Parker_Velocity_Control_DW.DiscreteTimeIntegrator_PrevRese <= 0)) ||
          ((!Parker_Velocity_Control_B.rtb_Compare_j) &&
           (Parker_Velocity_Control_DW.DiscreteTimeIntegrator_PrevRese == 1))) {
        Parker_Velocity_Control_DW.DiscreteTimeIntegrator_DSTATE =
          Parker_Velocity_Control_DW.BLUE_Rz;
      }

      // RelationalOperator: '<S486>/Compare' incorporates:
      //   Constant: '<S486>/Constant'
      //   DataStoreRead: '<S459>/Data Store Read2'

      Parker_Velocity_Control_B.rtb_Compare_n_m =
        (Parker_Velocity_Control_DW.BLACK_Rz ==
         Parker_Velocity_Control_P.Constant_Value_o);

      // DiscreteIntegrator: '<S459>/Discrete-Time Integrator1' incorporates:
      //   DataStoreRead: '<S459>/Data Store Read2'
      //   DataStoreRead: '<S459>/Data Store Read4'
      //   DataStoreWrite: '<S459>/RED_Px1'

      if (Parker_Velocity_Control_DW.DiscreteTimeIntegrator1_IC_LOAD != 0) {
        Parker_Velocity_Control_DW.DiscreteTimeIntegrator1_DSTATE =
          Parker_Velocity_Control_DW.BLACK_Rz;
      }

      if ((Parker_Velocity_Control_B.rtb_Compare_n_m &&
           (Parker_Velocity_Control_DW.DiscreteTimeIntegrator1_PrevRes <= 0)) ||
          ((!Parker_Velocity_Control_B.rtb_Compare_n_m) &&
           (Parker_Velocity_Control_DW.DiscreteTimeIntegrator1_PrevRes == 1))) {
        Parker_Velocity_Control_DW.DiscreteTimeIntegrator1_DSTATE =
          Parker_Velocity_Control_DW.BLACK_Rz;
      }

      Parker_Velocity_Control_DW.BLACK_IMU_Psi =
        Parker_Velocity_Control_P.DiscreteTimeIntegrator1_gainval *
        Parker_Velocity_Control_DW.BLACK_IMU_R +
        Parker_Velocity_Control_DW.DiscreteTimeIntegrator1_DSTATE;

      // End of DiscreteIntegrator: '<S459>/Discrete-Time Integrator1'

      // RelationalOperator: '<S487>/Compare' incorporates:
      //   Constant: '<S487>/Constant'
      //   DataStoreRead: '<S459>/Data Store Read5'

      Parker_Velocity_Control_B.rtb_Compare_f_m =
        (Parker_Velocity_Control_DW.RED_Rz ==
         Parker_Velocity_Control_P.Constant_Value_i);

      // DiscreteIntegrator: '<S459>/Discrete-Time Integrator2' incorporates:
      //   DataStoreRead: '<S459>/Data Store Read5'
      //   DataStoreRead: '<S459>/Data Store Read6'
      //   DataStoreWrite: '<S459>/RED_Px2'

      if (Parker_Velocity_Control_DW.DiscreteTimeIntegrator2_IC_LOAD != 0) {
        Parker_Velocity_Control_DW.DiscreteTimeIntegrator2_DSTATE =
          Parker_Velocity_Control_DW.RED_Rz;
      }

      if ((Parker_Velocity_Control_B.rtb_Compare_f_m &&
           (Parker_Velocity_Control_DW.DiscreteTimeIntegrator2_PrevRes <= 0)) ||
          ((!Parker_Velocity_Control_B.rtb_Compare_f_m) &&
           (Parker_Velocity_Control_DW.DiscreteTimeIntegrator2_PrevRes == 1))) {
        Parker_Velocity_Control_DW.DiscreteTimeIntegrator2_DSTATE =
          Parker_Velocity_Control_DW.RED_Rz;
      }

      Parker_Velocity_Control_DW.RED_IMU_Psi =
        Parker_Velocity_Control_P.DiscreteTimeIntegrator2_gainval *
        Parker_Velocity_Control_DW.RED_IMU_R +
        Parker_Velocity_Control_DW.DiscreteTimeIntegrator2_DSTATE;

      // End of DiscreteIntegrator: '<S459>/Discrete-Time Integrator2'

      // If: '<S460>/If' incorporates:
      //   Constant: '<S460>/Constant'
      //   DiscreteIntegrator: '<S499>/Acceleration  to Velocity'
      //   DiscreteIntegrator: '<S499>/Velocity to Position'

      if ((Parker_Velocity_Control_P.platformSelection == 4.0) ||
          (Parker_Velocity_Control_P.platformSelection == 5.0) ||
          (Parker_Velocity_Control_P.platformSelection == 10.0) ||
          (Parker_Velocity_Control_P.platformSelection == 11.0)) {
        // Outputs for IfAction SubSystem: '<S460>/RED + ARM' incorporates:
        //   ActionPort: '<S488>/Action Port'

        // Delay: '<S490>/Delay'
        rtb_Delay_0 = &Parker_Velocity_Control_DW.Delay_DSTATE[0];

        // MATLAB Function: '<S490>/Inertia Function ' incorporates:
        //   MATLAB Function: '<S490>/Coriolis Function'

        // MATLAB Function 'Simulate Plant Dynamics/RED and ARM Dynamics/RED + ARM/RED + ARM Dynamics/Inertia Function ': '<S493>:1' 
        // '<S493>:1:8' q0 = x(3);
        // '<S493>:1:9' q1 = x(4);
        // '<S493>:1:10' q2 = x(5);
        // '<S493>:1:11' q3 = x(6);
        // '<S493>:1:13' m0= model_param(1);
        // '<S493>:1:14' I0= model_param(2);
        // '<S493>:1:15' phi= model_param(7);
        // '<S493>:1:16' b0= model_param(8);
        // '<S493>:1:17' a1= model_param(9);
        // '<S493>:1:18' b1= model_param(10);
        // '<S493>:1:19' a2= model_param(11);
        // '<S493>:1:20' b2= model_param(12);
        // '<S493>:1:21' a3= model_param(13);
        // '<S493>:1:22' b3= model_param(14);
        // '<S493>:1:23' m1= model_param(15);
        // '<S493>:1:24' m2= model_param(16);
        // '<S493>:1:25' m3= model_param(17);
        // '<S493>:1:26' I1= model_param(18);
        // '<S493>:1:27' I2= model_param(19);
        // '<S493>:1:28' I3= model_param(20);
        // '<S493>:1:30' t2 = cos(q2);
        Parker_Velocity_Control_B.t2 = cos(rtb_Delay_0[4]);

        // '<S493>:1:31' t3 = cos(q3);
        Parker_Velocity_Control_B.Sum6_l = cos(rtb_Delay_0[5]);

        // '<S493>:1:32' t4 = a1+b1;
        Parker_Velocity_Control_B.t4_tmp =
          Parker_Velocity_Control_P.model_param[8] +
          Parker_Velocity_Control_P.model_param[9];

        // '<S493>:1:33' t5 = a2+b2;
        // '<S493>:1:34' t6 = a1.*m1;
        Parker_Velocity_Control_B.t6 = Parker_Velocity_Control_P.model_param[8] *
          Parker_Velocity_Control_P.model_param[14];

        // '<S493>:1:35' t7 = a2.*m2;
        Parker_Velocity_Control_B.t7_tmp =
          Parker_Velocity_Control_P.model_param[10] *
          Parker_Velocity_Control_P.model_param[15];

        // '<S493>:1:36' t8 = a2.*m3;
        Parker_Velocity_Control_B.t155 = Parker_Velocity_Control_P.model_param
          [10] * Parker_Velocity_Control_P.model_param[16];

        // '<S493>:1:37' t9 = a3.*m3;
        Parker_Velocity_Control_B.t9_tmp =
          Parker_Velocity_Control_P.model_param[12] *
          Parker_Velocity_Control_P.model_param[16];

        // '<S493>:1:38' t10 = b0.*m1;
        Parker_Velocity_Control_B.t7 = Parker_Velocity_Control_P.model_param[7] *
          Parker_Velocity_Control_P.model_param[14];

        // '<S493>:1:39' t11 = b0.*m2;
        Parker_Velocity_Control_B.t9 = Parker_Velocity_Control_P.model_param[7] *
          Parker_Velocity_Control_P.model_param[15];

        // '<S493>:1:40' t12 = b0.*m3;
        Parker_Velocity_Control_B.rtb_TmpSignalConversionAtSFu_jw =
          Parker_Velocity_Control_P.model_param[7] *
          Parker_Velocity_Control_P.model_param[16];

        // '<S493>:1:41' t13 = b2.*m3;
        Parker_Velocity_Control_B.t156 = Parker_Velocity_Control_P.model_param
          [11] * Parker_Velocity_Control_P.model_param[16];

        // '<S493>:1:42' t14 = phi+q0;
        Parker_Velocity_Control_B.t14_tmp =
          Parker_Velocity_Control_P.model_param[6] + rtb_Delay_0[2];

        // '<S493>:1:43' t15 = q0+q1;
        Parker_Velocity_Control_B.t15_tmp = rtb_Delay_0[2] + rtb_Delay_0[3];

        // '<S493>:1:44' t16 = q2+q3;
        Parker_Velocity_Control_B.t4 = rtb_Delay_0[4] + rtb_Delay_0[5];

        // '<S493>:1:45' t17 = a1.^2;
        Parker_Velocity_Control_B.t17 = Parker_Velocity_Control_P.model_param[8]
          * Parker_Velocity_Control_P.model_param[8];

        // '<S493>:1:46' t18 = a2.^2;
        // '<S493>:1:47' t19 = a3.^2;
        // '<S493>:1:48' t20 = b0.^2;
        // '<S493>:1:49' t21 = b1.^2;
        Parker_Velocity_Control_B.t21 = Parker_Velocity_Control_P.model_param[9]
          * Parker_Velocity_Control_P.model_param[9];

        // '<S493>:1:50' t22 = b2.^2;
        // '<S493>:1:51' t48 = -phi;
        // '<S493>:1:52' t49 = -q1;
        // '<S493>:1:53' t50 = m0+m1+m2+m3;
        Parker_Velocity_Control_B.Sum6_d =
          ((Parker_Velocity_Control_P.model_param[0] +
            Parker_Velocity_Control_P.model_param[14]) +
           Parker_Velocity_Control_P.model_param[15]) +
          Parker_Velocity_Control_P.model_param[16];

        // '<S493>:1:54' t76 = a1.*b1.*m2.*2.0;
        // '<S493>:1:55' t77 = a1.*b1.*m3.*2.0;
        // '<S493>:1:56' t23 = a1.*t7;
        Parker_Velocity_Control_B.t23 = Parker_Velocity_Control_P.model_param[8]
          * Parker_Velocity_Control_B.t7_tmp;

        // '<S493>:1:57' t24 = a1.*t8;
        Parker_Velocity_Control_B.t24 = Parker_Velocity_Control_P.model_param[8]
          * Parker_Velocity_Control_B.t155;

        // '<S493>:1:58' t25 = a1.*t9;
        Parker_Velocity_Control_B.t25 = Parker_Velocity_Control_P.model_param[8]
          * Parker_Velocity_Control_B.t9_tmp;

        // '<S493>:1:59' t26 = a3.*t8;
        Parker_Velocity_Control_B.rtb_Sum_h_idx_1 =
          Parker_Velocity_Control_P.model_param[12] *
          Parker_Velocity_Control_B.t155;

        // '<S493>:1:60' t27 = b0.*t6;
        // '<S493>:1:61' t28 = a1.*t11;
        // '<S493>:1:62' t29 = a1.*t12;
        // '<S493>:1:63' t30 = b0.*t7;
        Parker_Velocity_Control_B.t30 = Parker_Velocity_Control_P.model_param[7]
          * Parker_Velocity_Control_B.t7_tmp;

        // '<S493>:1:64' t31 = b0.*t8;
        Parker_Velocity_Control_B.t31 = Parker_Velocity_Control_P.model_param[7]
          * Parker_Velocity_Control_B.t155;

        // '<S493>:1:65' t32 = b1.*t7;
        Parker_Velocity_Control_B.rtb_Sum_h_idx_0 =
          Parker_Velocity_Control_P.model_param[9] *
          Parker_Velocity_Control_B.t7_tmp;

        // '<S493>:1:66' t33 = a1.*t13;
        Parker_Velocity_Control_B.t33 = Parker_Velocity_Control_P.model_param[8]
          * Parker_Velocity_Control_B.t156;

        // '<S493>:1:67' t34 = b1.*t8;
        Parker_Velocity_Control_B.t34 = Parker_Velocity_Control_P.model_param[9]
          * Parker_Velocity_Control_B.t155;

        // '<S493>:1:68' t35 = b0.*t9;
        Parker_Velocity_Control_B.t35 = Parker_Velocity_Control_P.model_param[7]
          * Parker_Velocity_Control_B.t9_tmp;

        // '<S493>:1:69' t36 = b1.*t9;
        Parker_Velocity_Control_B.rtb_x_ddot_idx_1 =
          Parker_Velocity_Control_P.model_param[9] *
          Parker_Velocity_Control_B.t9_tmp;

        // '<S493>:1:70' t37 = b2.*t9;
        Parker_Velocity_Control_B.t37 = Parker_Velocity_Control_P.model_param[11]
          * Parker_Velocity_Control_B.t9_tmp;

        // '<S493>:1:71' t38 = b1.*t11;
        // '<S493>:1:72' t39 = b1.*t12;
        // '<S493>:1:73' t40 = b2.*t12;
        Parker_Velocity_Control_B.rtb_TmpSignalConversionAtSFun_d =
          Parker_Velocity_Control_P.model_param[11] *
          Parker_Velocity_Control_B.rtb_TmpSignalConversionAtSFu_jw;

        // '<S493>:1:74' t41 = b1.*t13;
        Parker_Velocity_Control_B.t41 = Parker_Velocity_Control_P.model_param[9]
          * Parker_Velocity_Control_B.t156;

        // '<S493>:1:75' t42 = cos(t14);
        // '<S493>:1:76' t43 = cos(t15);
        // '<S493>:1:77' t44 = cos(t16);
        Parker_Velocity_Control_B.t44 = cos(Parker_Velocity_Control_B.t4);

        // '<S493>:1:78' t45 = q2+t15;
        Parker_Velocity_Control_B.t45 = rtb_Delay_0[4] +
          Parker_Velocity_Control_B.t15_tmp;

        // '<S493>:1:79' t46 = sin(t14);
        // '<S493>:1:80' t47 = sin(t15);
        // '<S493>:1:81' t52 = t15+t16;
        Parker_Velocity_Control_B.t52 = Parker_Velocity_Control_B.t15_tmp +
          Parker_Velocity_Control_B.t4;

        // '<S493>:1:82' t54 = -t6;
        // '<S493>:1:83' t55 = -t7;
        // '<S493>:1:84' t56 = -t8;
        // '<S493>:1:85' t57 = -t9;
        // '<S493>:1:86' t58 = -t10;
        // '<S493>:1:87' t59 = -t11;
        // '<S493>:1:88' t60 = -t12;
        // '<S493>:1:89' t61 = -t13;
        // '<S493>:1:90' t62 = a1.*t6;
        // '<S493>:1:91' t63 = m2.*t17;
        // '<S493>:1:92' t64 = m3.*t17;
        // '<S493>:1:93' t65 = a2.*t7;
        // '<S493>:1:94' t66 = a2.*t8;
        // '<S493>:1:95' t67 = a3.*t9;
        // '<S493>:1:96' t69 = m2.*t21;
        // '<S493>:1:97' t70 = m3.*t21;
        // '<S493>:1:98' t71 = b2.*t13;
        // '<S493>:1:99' t81 = b2.*t8.*2.0;
        // '<S493>:1:100' t85 = m2.*t4;
        // '<S493>:1:101' t86 = m3.*t4;
        // '<S493>:1:102' t87 = m3.*t5;
        Parker_Velocity_Control_B.rtb_Sum_h_idx_2 =
          Parker_Velocity_Control_P.model_param[10] +
          Parker_Velocity_Control_P.model_param[11];

        // '<S493>:1:103' t94 = phi+t49;
        // '<S493>:1:104' t95 = q1+q2+t48;
        // '<S493>:1:105' t101 = q1+t16+t48;
        // '<S493>:1:106' t106 = t10+t11+t12;
        Parker_Velocity_Control_B.t106 = (Parker_Velocity_Control_B.t7 +
          Parker_Velocity_Control_B.t9) +
          Parker_Velocity_Control_B.rtb_TmpSignalConversionAtSFu_jw;

        // '<S493>:1:107' t51 = cos(t45);
        Parker_Velocity_Control_B.t51 = cos(Parker_Velocity_Control_B.t45);

        // '<S493>:1:108' t53 = sin(t45);
        Parker_Velocity_Control_B.t53 = sin(Parker_Velocity_Control_B.t45);

        // '<S493>:1:109' t68 = t67;
        // '<S493>:1:110' t72 = t23.*2.0;
        // '<S493>:1:111' t73 = t24.*2.0;
        // '<S493>:1:112' t74 = t25.*2.0;
        // '<S493>:1:113' t75 = t26.*2.0;
        // '<S493>:1:114' t78 = t32.*2.0;
        // '<S493>:1:115' t79 = t33.*2.0;
        // '<S493>:1:116' t80 = t34.*2.0;
        // '<S493>:1:117' t82 = t36.*2.0;
        // '<S493>:1:118' t83 = t37.*2.0;
        // '<S493>:1:119' t84 = t41.*2.0;
        // '<S493>:1:120' t88 = cos(t52);
        // '<S493>:1:121' t89 = -t30;
        // '<S493>:1:122' t90 = -t31;
        // '<S493>:1:123' t91 = -t35;
        // '<S493>:1:124' t92 = -t40;
        // '<S493>:1:125' t93 = sin(t52);
        // '<S493>:1:126' t96 = sin(t94);
        // '<S493>:1:127' t98 = -t85;
        // '<S493>:1:128' t99 = -t86;
        // '<S493>:1:129' t100 = -t87;
        // '<S493>:1:130' t102 = sin(t95);
        Parker_Velocity_Control_B.t174 = (rtb_Delay_0[3] + rtb_Delay_0[4]) +
          -Parker_Velocity_Control_P.model_param[6];
        Parker_Velocity_Control_B.t102 = sin(Parker_Velocity_Control_B.t174);

        // '<S493>:1:131' t103 = t25+t36;
        // '<S493>:1:132' t104 = t26+t37;
        // '<S493>:1:133' t105 = sin(t101);
        Parker_Velocity_Control_B.t105 = sin((rtb_Delay_0[3] +
          Parker_Velocity_Control_B.t4) +
          -Parker_Velocity_Control_P.model_param[6]);

        // '<S493>:1:134' t115 = t42.*t106;
        // '<S493>:1:135' t118 = t55+t56+t61;
        // '<S493>:1:136' t119 = t58+t59+t60;
        // '<S493>:1:137' t125 = t27+t28+t29+t38+t39;
        // '<S493>:1:138' t126 = -t46.*t106;
        // '<S493>:1:139' t129 = -t43.*(t6+t85+t86);
        // '<S493>:1:140' t130 = -t47.*(t6+t85+t86);
        // '<S493>:1:141' t131 = t23+t24+t32+t33+t34+t41;
        // '<S493>:1:142' t97 = I3+t68;
        Parker_Velocity_Control_B.t97 = Parker_Velocity_Control_P.model_param[12]
          * Parker_Velocity_Control_B.t9_tmp +
          Parker_Velocity_Control_P.model_param[19];

        // '<S493>:1:143' t107 = t74+t82;
        // '<S493>:1:144' t108 = t75+t83;
        // '<S493>:1:145' t109 = t57.*t88;
        Parker_Velocity_Control_B.t109 = -Parker_Velocity_Control_B.t9_tmp * cos
          (Parker_Velocity_Control_B.t52);

        // '<S493>:1:146' t110 = t57.*t93;
        Parker_Velocity_Control_B.t110 = -Parker_Velocity_Control_B.t9_tmp * sin
          (Parker_Velocity_Control_B.t52);

        // '<S493>:1:147' t111 = t3.*t104;
        // '<S493>:1:148' t112 = t55+t100;
        // '<S493>:1:149' t113 = t44.*t103;
        Parker_Velocity_Control_B.t113 = (Parker_Velocity_Control_B.t25 +
          Parker_Velocity_Control_B.rtb_x_ddot_idx_1) *
          Parker_Velocity_Control_B.t44;

        // '<S493>:1:150' t117 = t91.*t105;
        Parker_Velocity_Control_B.t117 = -Parker_Velocity_Control_B.t35 *
          Parker_Velocity_Control_B.t105;

        // '<S493>:1:151' t120 = -t53.*(t7+t87);
        // '<S493>:1:152' t121 = t89+t90+t92;
        // '<S493>:1:153' t122 = -t51.*(t7+t87);
        // '<S493>:1:154' t124 = t54+t98+t99;
        // '<S493>:1:155' t127 = -t51.*(t7+t8+t13);
        // '<S493>:1:156' t128 = -t53.*(t7+t8+t13);
        // '<S493>:1:157' t133 = t2.*t131;
        // '<S493>:1:158' t134 = -t102.*(t30+t31+t40);
        Parker_Velocity_Control_B.t45 = ((Parker_Velocity_Control_B.t30 +
          Parker_Velocity_Control_B.t31) +
          Parker_Velocity_Control_B.rtb_TmpSignalConversionAtSFun_d) *
          -Parker_Velocity_Control_B.t102;

        // '<S493>:1:159' t135 = t96.*t125;
        Parker_Velocity_Control_B.t127 = Parker_Velocity_Control_P.model_param[6]
          + -rtb_Delay_0[3];
        Parker_Velocity_Control_B.t135 =
          ((((Parker_Velocity_Control_P.model_param[7] *
              Parker_Velocity_Control_B.t6 +
              Parker_Velocity_Control_P.model_param[8] *
              Parker_Velocity_Control_B.t9) +
             Parker_Velocity_Control_P.model_param[8] *
             Parker_Velocity_Control_B.rtb_TmpSignalConversionAtSFu_jw) +
            Parker_Velocity_Control_P.model_param[9] *
            Parker_Velocity_Control_B.t9) +
           Parker_Velocity_Control_P.model_param[9] *
           Parker_Velocity_Control_B.rtb_TmpSignalConversionAtSFu_jw) * sin
          (Parker_Velocity_Control_B.t127);

        // '<S493>:1:160' t136 = t72+t73+t78+t79+t80+t84;
        // '<S493>:1:161' t114 = t3.*t108;
        Parker_Velocity_Control_B.t114 =
          (Parker_Velocity_Control_B.rtb_Sum_h_idx_1 * 2.0 +
           Parker_Velocity_Control_B.t37 * 2.0) *
          Parker_Velocity_Control_B.Sum6_l;

        // '<S493>:1:162' t116 = t44.*t107;
        Parker_Velocity_Control_B.rtb_x_ddot_idx_1 =
          (Parker_Velocity_Control_B.t25 * 2.0 +
           Parker_Velocity_Control_B.rtb_x_ddot_idx_1 * 2.0) *
          Parker_Velocity_Control_B.t44;

        // '<S493>:1:163' t123 = t97+t111;
        Parker_Velocity_Control_B.Sum6_l =
          (Parker_Velocity_Control_B.rtb_Sum_h_idx_1 +
           Parker_Velocity_Control_B.t37) * Parker_Velocity_Control_B.Sum6_l +
          Parker_Velocity_Control_B.t97;

        // '<S493>:1:164' t132 = I2+t65+t66+t71+t81+t97;
        Parker_Velocity_Control_B.rtb_Sum_h_idx_1 =
          ((((Parker_Velocity_Control_P.model_param[10] *
              Parker_Velocity_Control_B.t7_tmp +
              Parker_Velocity_Control_P.model_param[18]) +
             Parker_Velocity_Control_P.model_param[10] *
             Parker_Velocity_Control_B.t155) +
            Parker_Velocity_Control_P.model_param[11] *
            Parker_Velocity_Control_B.t156) +
           Parker_Velocity_Control_P.model_param[11] *
           Parker_Velocity_Control_B.t155 * 2.0) + Parker_Velocity_Control_B.t97;

        // '<S493>:1:165' t138 = t2.*t136;
        Parker_Velocity_Control_B.t37 = (((((Parker_Velocity_Control_B.t23 * 2.0
          + Parker_Velocity_Control_B.t24 * 2.0) +
          Parker_Velocity_Control_B.rtb_Sum_h_idx_0 * 2.0) +
          Parker_Velocity_Control_B.t33 * 2.0) + Parker_Velocity_Control_B.t34 *
          2.0) + Parker_Velocity_Control_B.t41 * 2.0) *
          Parker_Velocity_Control_B.t2;

        // '<S493>:1:166' t139 = t109+t127;
        Parker_Velocity_Control_B.t167 = (Parker_Velocity_Control_B.t7_tmp +
          Parker_Velocity_Control_B.t155) + Parker_Velocity_Control_B.t156;
        Parker_Velocity_Control_B.t25 = Parker_Velocity_Control_B.t167 *
          -Parker_Velocity_Control_B.t51 + Parker_Velocity_Control_B.t109;

        // '<S493>:1:167' t140 = t110+t128;
        Parker_Velocity_Control_B.t8 = Parker_Velocity_Control_B.t167 *
          -Parker_Velocity_Control_B.t53 + Parker_Velocity_Control_B.t110;

        // '<S493>:1:168' t142 = t110+t120+t130;
        Parker_Velocity_Control_B.t52 = Parker_Velocity_Control_B.t7_tmp +
          Parker_Velocity_Control_B.rtb_Sum_h_idx_2 *
          Parker_Velocity_Control_P.model_param[16];
        Parker_Velocity_Control_B.t14 = (Parker_Velocity_Control_B.t6 +
          Parker_Velocity_Control_P.model_param[15] *
          Parker_Velocity_Control_B.t4_tmp) +
          Parker_Velocity_Control_P.model_param[16] *
          Parker_Velocity_Control_B.t4_tmp;
        Parker_Velocity_Control_B.t53 = (Parker_Velocity_Control_B.t52 *
          -Parker_Velocity_Control_B.t53 + Parker_Velocity_Control_B.t110) +
          Parker_Velocity_Control_B.t14 * -sin(Parker_Velocity_Control_B.t15_tmp);

        // '<S493>:1:169' t143 = t109+t122+t129;
        Parker_Velocity_Control_B.t44 = (Parker_Velocity_Control_B.t52 *
          -Parker_Velocity_Control_B.t51 + Parker_Velocity_Control_B.t109) +
          Parker_Velocity_Control_B.t14 * -cos(Parker_Velocity_Control_B.t15_tmp);

        // '<S493>:1:170' t137 = t113+t123;
        Parker_Velocity_Control_B.t85 = Parker_Velocity_Control_B.t113 +
          Parker_Velocity_Control_B.Sum6_l;

        // '<S493>:1:171' t144 = I1+t62+t63+t64+t69+t70+t76+t77+t132;
        Parker_Velocity_Control_B.t51 = Parker_Velocity_Control_P.model_param[8]
          * Parker_Velocity_Control_P.model_param[9];
        Parker_Velocity_Control_B.t21 =
          (((((((Parker_Velocity_Control_P.model_param[8] *
                 Parker_Velocity_Control_B.t6 +
                 Parker_Velocity_Control_P.model_param[17]) +
                Parker_Velocity_Control_P.model_param[15] *
                Parker_Velocity_Control_B.t17) +
               Parker_Velocity_Control_P.model_param[16] *
               Parker_Velocity_Control_B.t17) +
              Parker_Velocity_Control_P.model_param[15] *
              Parker_Velocity_Control_B.t21) +
             Parker_Velocity_Control_P.model_param[16] *
             Parker_Velocity_Control_B.t21) + Parker_Velocity_Control_B.t51 *
            Parker_Velocity_Control_P.model_param[15] * 2.0) +
           Parker_Velocity_Control_B.t51 *
           Parker_Velocity_Control_P.model_param[16] * 2.0) +
          Parker_Velocity_Control_B.rtb_Sum_h_idx_1;

        // '<S493>:1:172' t145 = t115+t142;
        Parker_Velocity_Control_B.t51 = cos(Parker_Velocity_Control_B.t14_tmp) *
          Parker_Velocity_Control_B.t106 + Parker_Velocity_Control_B.t53;

        // '<S493>:1:173' t146 = t126+t143;
        Parker_Velocity_Control_B.t14 = -sin(Parker_Velocity_Control_B.t14_tmp) *
          Parker_Velocity_Control_B.t106 + Parker_Velocity_Control_B.t44;

        // '<S493>:1:174' t147 = t113+t114+t132+t133;
        Parker_Velocity_Control_B.t24 = (((((Parker_Velocity_Control_B.t23 +
          Parker_Velocity_Control_B.t24) +
          Parker_Velocity_Control_B.rtb_Sum_h_idx_0) +
          Parker_Velocity_Control_B.t33) + Parker_Velocity_Control_B.t34) +
          Parker_Velocity_Control_B.t41) * Parker_Velocity_Control_B.t2 +
          ((Parker_Velocity_Control_B.t113 + Parker_Velocity_Control_B.t114) +
           Parker_Velocity_Control_B.rtb_Sum_h_idx_1);

        // '<S493>:1:175' t141 = t117+t137;
        Parker_Velocity_Control_B.t106 = Parker_Velocity_Control_B.t117 +
          Parker_Velocity_Control_B.t85;

        // '<S493>:1:176' t148 = t117+t134+t147;
        Parker_Velocity_Control_B.t113 = (Parker_Velocity_Control_B.t117 +
          Parker_Velocity_Control_B.t45) + Parker_Velocity_Control_B.t24;

        // '<S493>:1:177' t149 = t114+t116+t117+t134+t135+t138+t144;
        Parker_Velocity_Control_B.t2 = Parker_Velocity_Control_B.t114 +
          Parker_Velocity_Control_B.rtb_x_ddot_idx_1;
        Parker_Velocity_Control_B.rtb_Sum_h_idx_0 =
          ((((Parker_Velocity_Control_B.t2 + Parker_Velocity_Control_B.t117) +
             Parker_Velocity_Control_B.t45) + Parker_Velocity_Control_B.t135) +
           Parker_Velocity_Control_B.t37) + Parker_Velocity_Control_B.t21;

        // '<S493>:1:178' InertiaS = reshape([t50,0.0,t146,t143,t139,t109,0.0,t50,t145,t142,t140,t110,t146,t145,I0+t114+t116+t135.*2.0+t138+t144+b0.*t10+b0.*t11+b0.*t12-t35.*t105.*2.0-t102.*(t30.*2.0+t31.*2.0+t40.*2.0),t149,t148,t141,t143,t142,t149,t114+t116+t138+t144,t147,t137,t139,t140,t148,t147,t114+t132,t123,t109,t110,t141,t137,t123,t97],[6,6]); 
        Parker_Velocity_Control_B.InertiaS[0] = Parker_Velocity_Control_B.Sum6_d;
        Parker_Velocity_Control_B.InertiaS[1] = 0.0;
        Parker_Velocity_Control_B.InertiaS[2] = Parker_Velocity_Control_B.t14;
        Parker_Velocity_Control_B.InertiaS[3] = Parker_Velocity_Control_B.t44;
        Parker_Velocity_Control_B.InertiaS[4] = Parker_Velocity_Control_B.t25;
        Parker_Velocity_Control_B.InertiaS[5] = Parker_Velocity_Control_B.t109;
        Parker_Velocity_Control_B.InertiaS[6] = 0.0;
        Parker_Velocity_Control_B.InertiaS[7] = Parker_Velocity_Control_B.Sum6_d;
        Parker_Velocity_Control_B.InertiaS[8] = Parker_Velocity_Control_B.t51;
        Parker_Velocity_Control_B.InertiaS[9] = Parker_Velocity_Control_B.t53;
        Parker_Velocity_Control_B.InertiaS[10] = Parker_Velocity_Control_B.t8;
        Parker_Velocity_Control_B.InertiaS[11] = Parker_Velocity_Control_B.t110;
        Parker_Velocity_Control_B.InertiaS[12] = Parker_Velocity_Control_B.t14;
        Parker_Velocity_Control_B.InertiaS[13] = Parker_Velocity_Control_B.t51;
        Parker_Velocity_Control_B.InertiaS[14] =
          (((((((((Parker_Velocity_Control_P.model_param[1] +
                   Parker_Velocity_Control_B.t114) +
                  Parker_Velocity_Control_B.rtb_x_ddot_idx_1) +
                 Parker_Velocity_Control_B.t135 * 2.0) +
                Parker_Velocity_Control_B.t37) + Parker_Velocity_Control_B.t21)
              + Parker_Velocity_Control_P.model_param[7] *
              Parker_Velocity_Control_B.t7) +
             Parker_Velocity_Control_P.model_param[7] *
             Parker_Velocity_Control_B.t9) +
            Parker_Velocity_Control_P.model_param[7] *
            Parker_Velocity_Control_B.rtb_TmpSignalConversionAtSFu_jw) -
           Parker_Velocity_Control_B.t35 * Parker_Velocity_Control_B.t105 * 2.0)
          - ((Parker_Velocity_Control_B.t30 * 2.0 +
              Parker_Velocity_Control_B.t31 * 2.0) +
             Parker_Velocity_Control_B.rtb_TmpSignalConversionAtSFun_d * 2.0) *
          Parker_Velocity_Control_B.t102;
        Parker_Velocity_Control_B.InertiaS[15] =
          Parker_Velocity_Control_B.rtb_Sum_h_idx_0;
        Parker_Velocity_Control_B.InertiaS[16] = Parker_Velocity_Control_B.t113;
        Parker_Velocity_Control_B.InertiaS[17] = Parker_Velocity_Control_B.t106;
        Parker_Velocity_Control_B.InertiaS[18] = Parker_Velocity_Control_B.t44;
        Parker_Velocity_Control_B.InertiaS[19] = Parker_Velocity_Control_B.t53;
        Parker_Velocity_Control_B.InertiaS[20] =
          Parker_Velocity_Control_B.rtb_Sum_h_idx_0;
        Parker_Velocity_Control_B.InertiaS[21] = (Parker_Velocity_Control_B.t2 +
          Parker_Velocity_Control_B.t37) + Parker_Velocity_Control_B.t21;
        Parker_Velocity_Control_B.InertiaS[22] = Parker_Velocity_Control_B.t24;
        Parker_Velocity_Control_B.InertiaS[23] = Parker_Velocity_Control_B.t85;
        Parker_Velocity_Control_B.InertiaS[24] = Parker_Velocity_Control_B.t25;
        Parker_Velocity_Control_B.InertiaS[25] = Parker_Velocity_Control_B.t8;
        Parker_Velocity_Control_B.InertiaS[26] = Parker_Velocity_Control_B.t113;
        Parker_Velocity_Control_B.InertiaS[27] = Parker_Velocity_Control_B.t24;
        Parker_Velocity_Control_B.InertiaS[28] = Parker_Velocity_Control_B.t114
          + Parker_Velocity_Control_B.rtb_Sum_h_idx_1;
        Parker_Velocity_Control_B.InertiaS[29] =
          Parker_Velocity_Control_B.Sum6_l;
        Parker_Velocity_Control_B.InertiaS[30] = Parker_Velocity_Control_B.t109;
        Parker_Velocity_Control_B.InertiaS[31] = Parker_Velocity_Control_B.t110;
        Parker_Velocity_Control_B.InertiaS[32] = Parker_Velocity_Control_B.t106;
        Parker_Velocity_Control_B.InertiaS[33] = Parker_Velocity_Control_B.t85;
        Parker_Velocity_Control_B.InertiaS[34] =
          Parker_Velocity_Control_B.Sum6_l;
        Parker_Velocity_Control_B.InertiaS[35] = Parker_Velocity_Control_B.t97;

        // MATLAB Function: '<S490>/Coriolis Function'
        // MATLAB Function 'Simulate Plant Dynamics/RED and ARM Dynamics/RED + ARM/RED + ARM Dynamics/Coriolis Function': '<S491>:1' 
        // '<S491>:1:7' q0 = x(3);
        // '<S491>:1:8' q1 = x(4);
        // '<S491>:1:9' q2 = x(5);
        // '<S491>:1:10' q3 = x(6);
        // '<S491>:1:12' q0_dot = x(9);
        // '<S491>:1:13' q1_dot = x(10);
        // '<S491>:1:14' q2_dot = x(11);
        // '<S491>:1:15' q3_dot = x(12);
        // '<S491>:1:17' m0= model_param(1);
        // '<S491>:1:18' I0= model_param(2);
        // '<S491>:1:19' phi= model_param(7);
        // '<S491>:1:20' b0= model_param(8);
        // '<S491>:1:21' a1= model_param(9);
        // '<S491>:1:22' b1= model_param(10);
        // '<S491>:1:23' a2= model_param(11);
        // '<S491>:1:24' b2= model_param(12);
        // '<S491>:1:25' a3= model_param(13);
        // '<S491>:1:26' b3= model_param(14);
        // '<S491>:1:27' m1= model_param(15);
        // '<S491>:1:28' m2= model_param(16);
        // '<S491>:1:29' m3= model_param(17);
        // '<S491>:1:30' I1= model_param(18);
        // '<S491>:1:31' I2= model_param(19);
        // '<S491>:1:32' I3= model_param(20);
        // '<S491>:1:34' t2 = sin(q2);
        Parker_Velocity_Control_B.t2 = sin(rtb_Delay_0[4]);

        // '<S491>:1:35' t3 = sin(q3);
        Parker_Velocity_Control_B.Sum6_l = sin(rtb_Delay_0[5]);

        // '<S491>:1:36' t4 = a1+b1;
        // '<S491>:1:37' t5 = a2+b2;
        // '<S491>:1:38' t6 = a1.*m1;
        // '<S491>:1:39' t7 = a2.*m2;
        // '<S491>:1:40' t8 = b0.*m1;
        // '<S491>:1:41' t9 = b0.*m2;
        // '<S491>:1:42' t10 = b0.*m3;
        // '<S491>:1:43' t11 = a1.*q0_dot;
        Parker_Velocity_Control_B.t21 = Parker_Velocity_Control_P.model_param[8]
          * rtb_Delay_0[8];

        // '<S491>:1:44' t12 = a1.*q1_dot;
        Parker_Velocity_Control_B.t85 = Parker_Velocity_Control_P.model_param[8]
          * rtb_Delay_0[9];

        // '<S491>:1:45' t13 = a2.*q0_dot;
        Parker_Velocity_Control_B.t35 = Parker_Velocity_Control_P.model_param[10]
          * rtb_Delay_0[8];

        // '<S491>:1:46' t14 = a2.*q1_dot;
        Parker_Velocity_Control_B.t14 = Parker_Velocity_Control_P.model_param[10]
          * rtb_Delay_0[9];

        // '<S491>:1:47' t15 = a2.*q2_dot;
        Parker_Velocity_Control_B.t37 = Parker_Velocity_Control_P.model_param[10]
          * rtb_Delay_0[10];

        // '<S491>:1:48' t16 = b1.*q0_dot;
        Parker_Velocity_Control_B.t16 = Parker_Velocity_Control_P.model_param[9]
          * rtb_Delay_0[8];

        // '<S491>:1:49' t17 = b1.*q1_dot;
        Parker_Velocity_Control_B.t17 = Parker_Velocity_Control_P.model_param[9]
          * rtb_Delay_0[9];

        // '<S491>:1:50' t18 = b2.*q0_dot;
        Parker_Velocity_Control_B.t18 = Parker_Velocity_Control_P.model_param[11]
          * rtb_Delay_0[8];

        // '<S491>:1:51' t19 = b2.*q1_dot;
        Parker_Velocity_Control_B.t19 = Parker_Velocity_Control_P.model_param[11]
          * rtb_Delay_0[9];

        // '<S491>:1:52' t20 = b2.*q2_dot;
        Parker_Velocity_Control_B.t20 = Parker_Velocity_Control_P.model_param[11]
          * rtb_Delay_0[10];

        // '<S491>:1:53' t21 = phi+q0;
        // '<S491>:1:54' t22 = q2+q3;
        // '<S491>:1:55' t23 = a3.*m3.*q0_dot;
        // '<S491>:1:56' t24 = a3.*m3.*q1_dot;
        // '<S491>:1:57' t25 = a3.*m3.*q2_dot;
        Parker_Velocity_Control_B.t25 = Parker_Velocity_Control_B.t9_tmp *
          rtb_Delay_0[10];

        // '<S491>:1:58' t26 = a3.*m3.*q3_dot;
        Parker_Velocity_Control_B.rtb_Sum_h_idx_1 =
          Parker_Velocity_Control_B.t9_tmp * rtb_Delay_0[11];

        // '<S491>:1:59' t28 = -phi;
        // '<S491>:1:60' t29 = -q1;
        // '<S491>:1:61' t71 = q0_dot+q1_dot+q2_dot+q3_dot;
        Parker_Velocity_Control_B.t41 = (rtb_Delay_0[8] + rtb_Delay_0[9]) +
          rtb_Delay_0[10];
        Parker_Velocity_Control_B.rtb_TmpSignalConversionAtSFun_d =
          Parker_Velocity_Control_B.t41 + rtb_Delay_0[11];

        // '<S491>:1:62' t72 = pi./2.0;
        // '<S491>:1:63' t27 = sin(t22);
        Parker_Velocity_Control_B.t27 = sin(Parker_Velocity_Control_B.t4);

        // '<S491>:1:64' t30 = b0.*q0_dot.*t6;
        Parker_Velocity_Control_B.t197 = Parker_Velocity_Control_P.model_param[7]
          * rtb_Delay_0[8];
        Parker_Velocity_Control_B.t30 = Parker_Velocity_Control_B.t197 *
          Parker_Velocity_Control_B.t6;

        // '<S491>:1:65' t31 = b0.*q1_dot.*t6;
        Parker_Velocity_Control_B.t53 = Parker_Velocity_Control_P.model_param[7]
          * rtb_Delay_0[9];
        Parker_Velocity_Control_B.t31 = Parker_Velocity_Control_B.t53 *
          Parker_Velocity_Control_B.t6;

        // '<S491>:1:66' t32 = t9.*t11;
        Parker_Velocity_Control_B.rtb_Sum_h_idx_0 = Parker_Velocity_Control_B.t9
          * Parker_Velocity_Control_B.t21;

        // '<S491>:1:67' t33 = t9.*t12;
        Parker_Velocity_Control_B.t33 = Parker_Velocity_Control_B.t9 *
          Parker_Velocity_Control_B.t85;

        // '<S491>:1:68' t34 = t10.*t11;
        Parker_Velocity_Control_B.t34 =
          Parker_Velocity_Control_B.rtb_TmpSignalConversionAtSFu_jw *
          Parker_Velocity_Control_B.t21;

        // '<S491>:1:69' t35 = b0.*q0_dot.*t7;
        // '<S491>:1:70' t36 = t10.*t12;
        Parker_Velocity_Control_B.rtb_x_ddot_idx_1 =
          Parker_Velocity_Control_B.rtb_TmpSignalConversionAtSFu_jw *
          Parker_Velocity_Control_B.t85;

        // '<S491>:1:71' t37 = b0.*q1_dot.*t7;
        // '<S491>:1:72' t38 = t10.*t13;
        // '<S491>:1:73' t39 = t7.*t16;
        // '<S491>:1:74' t40 = b2.*m3.*t11;
        // '<S491>:1:75' t41 = b0.*q2_dot.*t7;
        // '<S491>:1:76' t42 = t10.*t14;
        Parker_Velocity_Control_B.t102 =
          Parker_Velocity_Control_B.rtb_TmpSignalConversionAtSFu_jw *
          Parker_Velocity_Control_B.t14;

        // '<S491>:1:77' t43 = t7.*t17;
        // '<S491>:1:78' t44 = b1.*m3.*t13;
        // '<S491>:1:79' t45 = a3.*q0_dot.*t10;
        Parker_Velocity_Control_B.t45 = Parker_Velocity_Control_P.model_param[12]
          * rtb_Delay_0[8] *
          Parker_Velocity_Control_B.rtb_TmpSignalConversionAtSFu_jw;

        // '<S491>:1:80' t46 = b2.*m3.*t12;
        // '<S491>:1:81' t47 = t10.*t15;
        Parker_Velocity_Control_B.t105 =
          Parker_Velocity_Control_B.rtb_TmpSignalConversionAtSFu_jw *
          Parker_Velocity_Control_B.t37;

        // '<S491>:1:82' t48 = b1.*q2_dot.*t7;
        // '<S491>:1:83' t49 = b1.*m3.*t14;
        // '<S491>:1:84' t50 = a3.*q1_dot.*t10;
        Parker_Velocity_Control_B.Sum6_d =
          Parker_Velocity_Control_P.model_param[12] * rtb_Delay_0[9] *
          Parker_Velocity_Control_B.rtb_TmpSignalConversionAtSFu_jw;

        // '<S491>:1:85' t51 = a3.*m3.*t16;
        // '<S491>:1:86' t52 = a1.*m3.*t20;
        Parker_Velocity_Control_B.t157 = Parker_Velocity_Control_P.model_param[8]
          * Parker_Velocity_Control_P.model_param[16];

        // '<S491>:1:87' t53 = b1.*m3.*t15;
        Parker_Velocity_Control_B.t52 = Parker_Velocity_Control_P.model_param[9]
          * Parker_Velocity_Control_P.model_param[16];

        // '<S491>:1:88' t54 = a3.*q2_dot.*t10;
        Parker_Velocity_Control_B.t97 = Parker_Velocity_Control_P.model_param[12]
          * rtb_Delay_0[10] *
          Parker_Velocity_Control_B.rtb_TmpSignalConversionAtSFu_jw;

        // '<S491>:1:89' t55 = a3.*m3.*t17;
        // '<S491>:1:90' t56 = a3.*q3_dot.*t10;
        Parker_Velocity_Control_B.t109 = Parker_Velocity_Control_P.model_param
          [12] * rtb_Delay_0[11] *
          Parker_Velocity_Control_B.rtb_TmpSignalConversionAtSFu_jw;

        // '<S491>:1:91' t57 = b1.*t25;
        // '<S491>:1:92' t58 = b1.*t26;
        // '<S491>:1:93' t59 = b2.*t26;
        // '<S491>:1:94' t60 = t9.*t16;
        Parker_Velocity_Control_B.t110 = Parker_Velocity_Control_B.t9 *
          Parker_Velocity_Control_B.t16;

        // '<S491>:1:95' t61 = t9.*t17;
        Parker_Velocity_Control_B.t135 = Parker_Velocity_Control_B.t9 *
          Parker_Velocity_Control_B.t17;

        // '<S491>:1:96' t62 = t10.*t16;
        Parker_Velocity_Control_B.t114 =
          Parker_Velocity_Control_B.rtb_TmpSignalConversionAtSFu_jw *
          Parker_Velocity_Control_B.t16;

        // '<S491>:1:97' t63 = t10.*t17;
        Parker_Velocity_Control_B.t117 =
          Parker_Velocity_Control_B.rtb_TmpSignalConversionAtSFu_jw *
          Parker_Velocity_Control_B.t17;

        // '<S491>:1:98' t64 = t10.*t18;
        // '<S491>:1:99' t65 = t10.*t19;
        // '<S491>:1:100' t66 = b2.*m3.*t16;
        // '<S491>:1:101' t67 = t10.*t20;
        // '<S491>:1:102' t68 = b2.*m3.*t17;
        // '<S491>:1:103' t69 = b1.*m3.*t20;
        // '<S491>:1:104' t70 = q0+q1+t22;
        Parker_Velocity_Control_B.t44 = Parker_Velocity_Control_B.t15_tmp +
          Parker_Velocity_Control_B.t4;

        // '<S491>:1:105' t73 = m2.*t4;
        // '<S491>:1:106' t74 = m3.*t4;
        Parker_Velocity_Control_B.t23 = Parker_Velocity_Control_P.model_param[16]
          * Parker_Velocity_Control_B.t4_tmp;

        // '<S491>:1:107' t75 = m3.*t5;
        Parker_Velocity_Control_B.t75 = Parker_Velocity_Control_P.model_param[16]
          * Parker_Velocity_Control_B.rtb_Sum_h_idx_2;

        // '<S491>:1:108' t76 = t7.*t11;
        // '<S491>:1:109' t77 = t7.*t12;
        // '<S491>:1:110' t78 = a2.*m3.*t11;
        // '<S491>:1:111' t79 = a1.*q2_dot.*t7;
        // '<S491>:1:112' t80 = a2.*m3.*t12;
        // '<S491>:1:113' t81 = a3.*m3.*t11;
        // '<S491>:1:114' t82 = a1.*m3.*t15;
        // '<S491>:1:115' t83 = a3.*m3.*t12;
        // '<S491>:1:116' t84 = a1.*t25;
        // '<S491>:1:117' t85 = a1.*t26;
        // '<S491>:1:118' t86 = a2.*t26;
        // '<S491>:1:119' t87 = -t23;
        // '<S491>:1:120' t88 = -t24;
        // '<S491>:1:121' t89 = -t25;
        // '<S491>:1:122' t90 = -t26;
        // '<S491>:1:123' t91 = phi+t29;
        // '<S491>:1:124' t128 = q1+q2+t28;
        // '<S491>:1:125' t140 = a3.*m3.*t71;
        Parker_Velocity_Control_B.t8 = Parker_Velocity_Control_B.t9_tmp *
          Parker_Velocity_Control_B.rtb_TmpSignalConversionAtSFun_d;

        // '<S491>:1:126' t142 = q1+t22+t28;
        // '<S491>:1:127' t143 = q0+q1+t72;
        // '<S491>:1:128' t152 = t8+t9+t10;
        // '<S491>:1:129' t161 = t11+t12+t16+t17;
        // '<S491>:1:130' t180 = t13+t14+t15+t18+t19+t20;
        // '<S491>:1:131' t92 = -t31;
        // '<S491>:1:132' t93 = -t33;
        // '<S491>:1:133' t94 = -t35;
        // '<S491>:1:134' t95 = -t36;
        // '<S491>:1:135' t96 = -t37;
        // '<S491>:1:136' t97 = -t38;
        // '<S491>:1:137' t98 = -t39;
        // '<S491>:1:138' t99 = -t40;
        // '<S491>:1:139' t100 = -t41;
        // '<S491>:1:140' t101 = -t42;
        // '<S491>:1:141' t102 = -t43;
        // '<S491>:1:142' t103 = -t44;
        // '<S491>:1:143' t104 = -t45;
        // '<S491>:1:144' t105 = -t46;
        // '<S491>:1:145' t106 = -t47;
        // '<S491>:1:146' t107 = -t48;
        // '<S491>:1:147' t108 = -t49;
        // '<S491>:1:148' t109 = -t50;
        // '<S491>:1:149' t110 = -t51;
        // '<S491>:1:150' t111 = -t52;
        // '<S491>:1:151' t112 = -t53;
        // '<S491>:1:152' t113 = -t54;
        // '<S491>:1:153' t114 = -t55;
        // '<S491>:1:154' t115 = -t56;
        // '<S491>:1:155' t116 = -t57;
        // '<S491>:1:156' t117 = -t58;
        // '<S491>:1:157' t118 = -t59;
        // '<S491>:1:158' t119 = -t61;
        // '<S491>:1:159' t120 = -t63;
        // '<S491>:1:160' t121 = -t64;
        // '<S491>:1:161' t122 = -t65;
        // '<S491>:1:162' t123 = -t66;
        // '<S491>:1:163' t124 = -t67;
        // '<S491>:1:164' t125 = -t68;
        // '<S491>:1:165' t126 = -t69;
        // '<S491>:1:166' t127 = cos(t91);
        Parker_Velocity_Control_B.t127 = cos(Parker_Velocity_Control_B.t127);

        // '<S491>:1:167' t129 = -t76;
        // '<S491>:1:168' t130 = -t77;
        // '<S491>:1:169' t131 = -t78;
        // '<S491>:1:170' t132 = -t79;
        // '<S491>:1:171' t133 = -t80;
        // '<S491>:1:172' t134 = -t81;
        // '<S491>:1:173' t135 = -t82;
        // '<S491>:1:174' t136 = -t83;
        // '<S491>:1:175' t137 = -t84;
        // '<S491>:1:176' t138 = -t85;
        // '<S491>:1:177' t139 = -t86;
        // '<S491>:1:178' t141 = cos(t128);
        Parker_Velocity_Control_B.t106 = cos(Parker_Velocity_Control_B.t174);

        // '<S491>:1:179' t144 = cos(t143);
        // '<S491>:1:180' t145 = q2+t143;
        Parker_Velocity_Control_B.t51 = rtb_Delay_0[4] +
          (Parker_Velocity_Control_B.t15_tmp + 1.5707963267948966);

        // '<S491>:1:181' t146 = sin(t143);
        // '<S491>:1:182' t147 = t7+t75;
        Parker_Velocity_Control_B.t24 = Parker_Velocity_Control_B.t7_tmp +
          Parker_Velocity_Control_B.t75;

        // '<S491>:1:183' t148 = cos(t142);
        Parker_Velocity_Control_B.t113 = cos((rtb_Delay_0[3] +
          Parker_Velocity_Control_B.t4) +
          -Parker_Velocity_Control_P.model_param[6]);

        // '<S491>:1:184' t150 = t70+t72;
        // '<S491>:1:185' t158 = a3.*t71.*t74;
        // '<S491>:1:186' t159 = a3.*t71.*t75;
        // '<S491>:1:187' t160 = q0_dot.*t152;
        // '<S491>:1:188' t167 = t6+t73+t74;
        Parker_Velocity_Control_B.t167 = (Parker_Velocity_Control_P.model_param
          [15] * Parker_Velocity_Control_B.t4_tmp + Parker_Velocity_Control_B.t6)
          + Parker_Velocity_Control_B.t23;

        // '<S491>:1:189' t173 = a3.*m3.*t161;
        // '<S491>:1:190' t174 = t35+t38+t64;
        Parker_Velocity_Control_B.t174 = (Parker_Velocity_Control_B.t197 *
          Parker_Velocity_Control_B.t7_tmp +
          Parker_Velocity_Control_B.rtb_TmpSignalConversionAtSFu_jw *
          Parker_Velocity_Control_B.t35) +
          Parker_Velocity_Control_B.rtb_TmpSignalConversionAtSFu_jw *
          Parker_Velocity_Control_B.t18;

        // '<S491>:1:191' t179 = -t3.*(t59+t86);
        Parker_Velocity_Control_B.t6 = (Parker_Velocity_Control_P.model_param[11]
          * Parker_Velocity_Control_B.rtb_Sum_h_idx_1 +
          Parker_Velocity_Control_P.model_param[10] *
          Parker_Velocity_Control_B.rtb_Sum_h_idx_1) *
          -Parker_Velocity_Control_B.Sum6_l;

        // '<S491>:1:192' t181 = t51+t55+t81+t83;
        Parker_Velocity_Control_B.t181 = ((Parker_Velocity_Control_B.t9_tmp *
          Parker_Velocity_Control_B.t16 + Parker_Velocity_Control_B.t9_tmp *
          Parker_Velocity_Control_B.t17) + Parker_Velocity_Control_B.t9_tmp *
          Parker_Velocity_Control_B.t21) + Parker_Velocity_Control_B.t9_tmp *
          Parker_Velocity_Control_B.t85;

        // '<S491>:1:193' t183 = a3.*m3.*t180;
        // '<S491>:1:194' t185 = t87+t88+t89+t90;
        // '<S491>:1:195' t190 = -t27.*(t57+t58+t84+t85);
        Parker_Velocity_Control_B.t4_tmp =
          ((Parker_Velocity_Control_P.model_param[9] *
            Parker_Velocity_Control_B.t25 +
            Parker_Velocity_Control_P.model_param[9] *
            Parker_Velocity_Control_B.rtb_Sum_h_idx_1) +
           Parker_Velocity_Control_P.model_param[8] *
           Parker_Velocity_Control_B.t25) +
          Parker_Velocity_Control_P.model_param[8] *
          Parker_Velocity_Control_B.rtb_Sum_h_idx_1;
        Parker_Velocity_Control_B.t4 = Parker_Velocity_Control_B.t4_tmp *
          -Parker_Velocity_Control_B.t27;

        // '<S491>:1:196' t197 = -t2.*(t48+t52+t53+t69+t79+t82);
        Parker_Velocity_Control_B.t197_tmp =
          ((((Parker_Velocity_Control_P.model_param[9] * rtb_Delay_0[10] *
              Parker_Velocity_Control_B.t7_tmp + Parker_Velocity_Control_B.t157 *
              Parker_Velocity_Control_B.t20) + Parker_Velocity_Control_B.t52 *
             Parker_Velocity_Control_B.t37) + Parker_Velocity_Control_B.t52 *
            Parker_Velocity_Control_B.t20) +
           Parker_Velocity_Control_P.model_param[8] * rtb_Delay_0[10] *
           Parker_Velocity_Control_B.t7_tmp) + Parker_Velocity_Control_B.t157 *
          Parker_Velocity_Control_B.t37;
        Parker_Velocity_Control_B.t197 = Parker_Velocity_Control_B.t197_tmp *
          -Parker_Velocity_Control_B.t2;

        // '<S491>:1:197' t203 = t39+t40+t43+t44+t46+t49+t66+t68+t76+t77+t78+t80; 
        Parker_Velocity_Control_B.t52 = ((((((((((Parker_Velocity_Control_B.t156
          * Parker_Velocity_Control_B.t21 + Parker_Velocity_Control_B.t7_tmp *
          Parker_Velocity_Control_B.t16) + Parker_Velocity_Control_B.t7_tmp *
          Parker_Velocity_Control_B.t17) + Parker_Velocity_Control_B.t52 *
          Parker_Velocity_Control_B.t35) + Parker_Velocity_Control_B.t156 *
          Parker_Velocity_Control_B.t85) + Parker_Velocity_Control_B.t52 *
          Parker_Velocity_Control_B.t14) + Parker_Velocity_Control_B.t156 *
          Parker_Velocity_Control_B.t16) + Parker_Velocity_Control_B.t156 *
          Parker_Velocity_Control_B.t17) + Parker_Velocity_Control_B.t7_tmp *
          Parker_Velocity_Control_B.t21) + Parker_Velocity_Control_B.t7_tmp *
          Parker_Velocity_Control_B.t85) + Parker_Velocity_Control_B.t155 *
          Parker_Velocity_Control_B.t21) + Parker_Velocity_Control_B.t155 *
          Parker_Velocity_Control_B.t85;

        // '<S491>:1:198' t149 = cos(t145);
        // '<S491>:1:199' t151 = sin(t145);
        // '<S491>:1:200' t153 = cos(t150);
        // '<S491>:1:201' t154 = sin(t150);
        // '<S491>:1:202' t155 = q0_dot.*t147;
        Parker_Velocity_Control_B.t155 = rtb_Delay_0[8] *
          Parker_Velocity_Control_B.t24;

        // '<S491>:1:203' t156 = q1_dot.*t147;
        Parker_Velocity_Control_B.t156 = rtb_Delay_0[9] *
          Parker_Velocity_Control_B.t24;

        // '<S491>:1:204' t157 = q2_dot.*t147;
        Parker_Velocity_Control_B.t157 = rtb_Delay_0[10] *
          Parker_Velocity_Control_B.t24;

        // '<S491>:1:205' t165 = -t158;
        // '<S491>:1:206' t166 = -t159;
        // '<S491>:1:207' t168 = -t160;
        Parker_Velocity_Control_B.t7 = -(((Parker_Velocity_Control_B.t7 +
          Parker_Velocity_Control_B.t9) +
          Parker_Velocity_Control_B.rtb_TmpSignalConversionAtSFu_jw) *
          rtb_Delay_0[8]);

        // '<S491>:1:208' t169 = t45.*t148;
        Parker_Velocity_Control_B.t24 = Parker_Velocity_Control_B.t45 *
          Parker_Velocity_Control_B.t113;

        // '<S491>:1:209' t170 = t118+t139;
        // '<S491>:1:210' t171 = q0_dot.*t167;
        Parker_Velocity_Control_B.t9 = rtb_Delay_0[8] *
          Parker_Velocity_Control_B.t167;

        // '<S491>:1:211' t172 = q1_dot.*t167;
        Parker_Velocity_Control_B.t167 *= rtb_Delay_0[9];

        // '<S491>:1:212' t182 = t27.*t173;
        Parker_Velocity_Control_B.t21 = (((Parker_Velocity_Control_B.t21 +
          Parker_Velocity_Control_B.t85) + Parker_Velocity_Control_B.t16) +
          Parker_Velocity_Control_B.t17) * Parker_Velocity_Control_B.t9_tmp *
          Parker_Velocity_Control_B.t27;

        // '<S491>:1:213' t184 = t141.*t174;
        // '<S491>:1:214' t186 = t3.*t183;
        Parker_Velocity_Control_B.t14 = (((((Parker_Velocity_Control_B.t35 +
          Parker_Velocity_Control_B.t14) + Parker_Velocity_Control_B.t37) +
          Parker_Velocity_Control_B.t18) + Parker_Velocity_Control_B.t19) +
          Parker_Velocity_Control_B.t20) * Parker_Velocity_Control_B.t9_tmp *
          Parker_Velocity_Control_B.Sum6_l;

        // '<S491>:1:215' t187 = t27.*t181;
        Parker_Velocity_Control_B.t37 = Parker_Velocity_Control_B.t27 *
          Parker_Velocity_Control_B.t181;

        // '<S491>:1:216' t188 = t116+t117+t137+t138;
        // '<S491>:1:217' t189 = t104+t109+t113+t115;
        // '<S491>:1:218' t194 = -t148.*(t45+t50+t54+t56);
        Parker_Velocity_Control_B.t35 = (((Parker_Velocity_Control_B.t45 +
          Parker_Velocity_Control_B.Sum6_d) + Parker_Velocity_Control_B.t97) +
          Parker_Velocity_Control_B.t109) * -Parker_Velocity_Control_B.t113;

        // '<S491>:1:219' t196 = t107+t111+t112+t126+t132+t135;
        // '<S491>:1:220' t204 = -t27.*(t57+t58+t84+t85+t181);
        Parker_Velocity_Control_B.t85 = (Parker_Velocity_Control_B.t4_tmp +
          Parker_Velocity_Control_B.t181) * -Parker_Velocity_Control_B.t27;

        // '<S491>:1:221' t205 = t94+t96+t97+t100+t101+t106+t121+t122+t124;
        // '<S491>:1:222' t206 = t2.*t203;
        Parker_Velocity_Control_B.t17 = Parker_Velocity_Control_B.t2 *
          Parker_Velocity_Control_B.t52;

        // '<S491>:1:223' t207 = -t141.*(t37+t41+t42+t47+t65+t67+t174);
        Parker_Velocity_Control_B.t4_tmp = ((((Parker_Velocity_Control_B.t53 *
          Parker_Velocity_Control_B.t7_tmp +
          Parker_Velocity_Control_P.model_param[7] * rtb_Delay_0[10] *
          Parker_Velocity_Control_B.t7_tmp) + Parker_Velocity_Control_B.t102) +
          Parker_Velocity_Control_B.t105) +
          Parker_Velocity_Control_B.rtb_TmpSignalConversionAtSFu_jw *
          Parker_Velocity_Control_B.t19) +
          Parker_Velocity_Control_B.rtb_TmpSignalConversionAtSFu_jw *
          Parker_Velocity_Control_B.t20;
        Parker_Velocity_Control_B.t45 = (Parker_Velocity_Control_B.t4_tmp +
          Parker_Velocity_Control_B.t174) * -Parker_Velocity_Control_B.t106;

        // '<S491>:1:224' t209 = -t2.*(t48+t52+t53+t69+t79+t82+t203);
        Parker_Velocity_Control_B.t2 = (Parker_Velocity_Control_B.t197_tmp +
          Parker_Velocity_Control_B.t52) * -Parker_Velocity_Control_B.t2;

        // '<S491>:1:225' t162 = -t155;
        // '<S491>:1:226' t163 = -t156;
        // '<S491>:1:227' t164 = -t157;
        // '<S491>:1:228' t175 = t3.*t166;
        Parker_Velocity_Control_B.t52 = Parker_Velocity_Control_P.model_param[12]
          * Parker_Velocity_Control_B.rtb_TmpSignalConversionAtSFun_d;
        Parker_Velocity_Control_B.t53 = -(Parker_Velocity_Control_B.t52 *
          Parker_Velocity_Control_B.t75) * Parker_Velocity_Control_B.Sum6_l;

        // '<S491>:1:229' t176 = -t171;
        // '<S491>:1:230' t177 = -t172;
        // '<S491>:1:231' t178 = t27.*t165;
        Parker_Velocity_Control_B.t52 = -(Parker_Velocity_Control_B.t52 *
          Parker_Velocity_Control_B.t23) * Parker_Velocity_Control_B.t27;

        // '<S491>:1:232' t191 = -t153.*(t23+t24+t25+t26);
        Parker_Velocity_Control_B.t25 = ((Parker_Velocity_Control_B.t9_tmp *
          rtb_Delay_0[8] + Parker_Velocity_Control_B.t9_tmp * rtb_Delay_0[9]) +
          Parker_Velocity_Control_B.t25) +
          Parker_Velocity_Control_B.rtb_Sum_h_idx_1;
        Parker_Velocity_Control_B.t27 = Parker_Velocity_Control_B.t25 * -cos
          (Parker_Velocity_Control_B.t44 + 1.5707963267948966);

        // '<S491>:1:233' t192 = -t154.*(t23+t24+t25+t26);
        Parker_Velocity_Control_B.t23 = Parker_Velocity_Control_B.t25 * -sin
          (Parker_Velocity_Control_B.t44 + 1.5707963267948966);

        // '<S491>:1:234' t198 = -t144.*(t171+t172);
        Parker_Velocity_Control_B.t9_tmp = Parker_Velocity_Control_B.t9 +
          Parker_Velocity_Control_B.t167;

        // '<S491>:1:235' t199 = -t146.*(t171+t172);
        // '<S491>:1:236' t200 = -t149.*(t155+t156+t157);
        Parker_Velocity_Control_B.t167 = (Parker_Velocity_Control_B.t155 +
          Parker_Velocity_Control_B.t156) + Parker_Velocity_Control_B.t157;
        Parker_Velocity_Control_B.t25 = Parker_Velocity_Control_B.t167 * -cos
          (Parker_Velocity_Control_B.t51);

        // '<S491>:1:237' t201 = -t151.*(t155+t156+t157);
        Parker_Velocity_Control_B.t51 = Parker_Velocity_Control_B.t167 * -sin
          (Parker_Velocity_Control_B.t51);

        // MATLAB Function: '<S490>/Dynamics Model'
        // '<S491>:1:238' t202 = t110+t114+t134+t136+t188;
        // '<S491>:1:239' t208 = t98+t99+t102+t103+t105+t108+t123+t125+t129+t130+t131+t133+t196; 
        // '<S491>:1:240' t193 = t176+t177;
        // '<S491>:1:241' t195 = t162+t163+t164;
        // '<S491>:1:242' CoriolisS = reshape([0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,t191+t198+t200+t168.*cos(t21),t192+t199+t201+t168.*sin(t21),t179+t190+t197-t127.*(t31+t33+t36+t61+t63)-t148.*(t50+t54+t56)-t141.*(t37+t41+t42+t47+t65+t67),t169+t179+t184+t190+t197+t127.*(t30+t32+t34+t60+t62),t169+t179+t184+t187+t206,t169+t182+t186,t191+t198+t200,t192+t199+t201,t179+t190+t194+t197+t207-t127.*(t30+t31+t32+t33+t34+t36+t60+t61+t62+t63),t179+t190+t197,t179+t187+t206,t182+t186,t191+t200,t192+t201,t179+t194+t204+t207+t209,t179+t204+t209,t3.*t5.*t90,a3.*t3.*t75.*(q0_dot+q1_dot+q2_dot),t140.*sin(t70),-t140.*cos(t70),t175+t178-a3.*t10.*t71.*t148,t175+t178,t175,0.0],[6,6]); 
        // MATLAB Function 'Simulate Plant Dynamics/RED and ARM Dynamics/RED + ARM/RED + ARM Dynamics/Dynamics Model': '<S492>:1' 
        // '<S492>:1:3' fqdot1 = Gamma1*(tanh(Gamma2*x(10)) - tanh(Gamma3*x(10))) + Gamma4*tanh(Gamma5*x(10)) + Gamma6*x(10); 
        // '<S492>:1:4' fqdot2 = Gamma1*(tanh(Gamma2*x(11)) - tanh(Gamma3*x(11))) + Gamma4*tanh(Gamma5*x(11)) + Gamma6*x(11); 
        // '<S492>:1:5' fqdot3 = Gamma1*(tanh(Gamma2*x(12)) - tanh(Gamma3*x(12))) + Gamma4*tanh(Gamma5*x(12)) + Gamma6*x(12); 
        // '<S492>:1:7' q_ddot = pinv(InertiaS)*(u - CoriolisS*x(7:12) - [0;0;0;fqdot1;fqdot2;fqdot3]); 
        Parker_Velocity_Control_B.p = true;
        for (Parker_Velocity_Control_B.i = 0; Parker_Velocity_Control_B.i < 36;
             Parker_Velocity_Control_B.i++) {
          Parker_Velocity_Control_B.X[Parker_Velocity_Control_B.i] = 0.0;
          if (Parker_Velocity_Control_B.p && (rtIsInf
               (Parker_Velocity_Control_B.InertiaS[Parker_Velocity_Control_B.i])
               || rtIsNaN
               (Parker_Velocity_Control_B.InertiaS[Parker_Velocity_Control_B.i])))
          {
            Parker_Velocity_Control_B.p = false;
          }
        }

        if (!Parker_Velocity_Control_B.p) {
          for (Parker_Velocity_Control_B.i = 0; Parker_Velocity_Control_B.i < 36;
               Parker_Velocity_Control_B.i++) {
            Parker_Velocity_Control_B.X[Parker_Velocity_Control_B.i] = (rtNaN);
          }
        } else {
          Parker_Velocity_Control_svd(Parker_Velocity_Control_B.InertiaS,
            Parker_Velocity_Control_B.U, Parker_Velocity_Control_B.s,
            Parker_Velocity_Control_B.V);
          Parker_Velocity_Control_B.t155 = fabs(Parker_Velocity_Control_B.s[0]);
          if ((!rtIsInf(Parker_Velocity_Control_B.t155)) && (!rtIsNaN
               (Parker_Velocity_Control_B.t155))) {
            if (Parker_Velocity_Control_B.t155 <= 2.2250738585072014E-308) {
              Parker_Velocity_Control_B.t155 = 4.94065645841247E-324;
            } else {
              frexp(Parker_Velocity_Control_B.t155,
                    &Parker_Velocity_Control_B.uElOffset1);
              Parker_Velocity_Control_B.t155 = ldexp(1.0,
                Parker_Velocity_Control_B.uElOffset1 - 53);
            }
          } else {
            Parker_Velocity_Control_B.t155 = (rtNaN);
          }

          Parker_Velocity_Control_B.t155 *= 6.0;
          Parker_Velocity_Control_B.i = -1;
          Parker_Velocity_Control_B.uElOffset1 = 0;
          while ((Parker_Velocity_Control_B.uElOffset1 < 6) &&
                 (Parker_Velocity_Control_B.s[Parker_Velocity_Control_B.uElOffset1]
                  > Parker_Velocity_Control_B.t155)) {
            Parker_Velocity_Control_B.i++;
            Parker_Velocity_Control_B.uElOffset1++;
          }

          if (Parker_Velocity_Control_B.i + 1 > 0) {
            Parker_Velocity_Control_B.vcol = 0;
            Parker_Velocity_Control_B.uElOffset1 = 0;
            while (Parker_Velocity_Control_B.uElOffset1 <=
                   Parker_Velocity_Control_B.i) {
              Parker_Velocity_Control_B.t155 = 1.0 /
                Parker_Velocity_Control_B.s[Parker_Velocity_Control_B.uElOffset1];
              Parker_Velocity_Control_B.ar = Parker_Velocity_Control_B.vcol;
              while (Parker_Velocity_Control_B.ar + 1 <=
                     Parker_Velocity_Control_B.vcol + 6) {
                Parker_Velocity_Control_B.V[Parker_Velocity_Control_B.ar] *=
                  Parker_Velocity_Control_B.t155;
                Parker_Velocity_Control_B.ar++;
              }

              Parker_Velocity_Control_B.vcol += 6;
              Parker_Velocity_Control_B.uElOffset1++;
            }

            for (Parker_Velocity_Control_B.uElOffset1 = 0;
                 Parker_Velocity_Control_B.uElOffset1 <= 31;
                 Parker_Velocity_Control_B.uElOffset1 += 6) {
              Parker_Velocity_Control_B.vcol =
                Parker_Velocity_Control_B.uElOffset1;
              while (Parker_Velocity_Control_B.vcol + 1 <=
                     Parker_Velocity_Control_B.uElOffset1 + 6) {
                Parker_Velocity_Control_B.X[Parker_Velocity_Control_B.vcol] =
                  0.0;
                Parker_Velocity_Control_B.vcol++;
              }
            }

            Parker_Velocity_Control_B.vcol = 0;
            for (Parker_Velocity_Control_B.uElOffset1 = 0;
                 Parker_Velocity_Control_B.uElOffset1 <= 31;
                 Parker_Velocity_Control_B.uElOffset1 += 6) {
              Parker_Velocity_Control_B.ar = -1;
              Parker_Velocity_Control_B.vcol++;
              Parker_Velocity_Control_B.ntIdx0 = 6 * Parker_Velocity_Control_B.i
                + Parker_Velocity_Control_B.vcol;
              Parker_Velocity_Control_B.ib = Parker_Velocity_Control_B.vcol;
              while (Parker_Velocity_Control_B.ib <=
                     Parker_Velocity_Control_B.ntIdx0) {
                Parker_Velocity_Control_B.ia = Parker_Velocity_Control_B.ar;
                Parker_Velocity_Control_B.b_ic =
                  Parker_Velocity_Control_B.uElOffset1;
                while (Parker_Velocity_Control_B.b_ic + 1 <=
                       Parker_Velocity_Control_B.uElOffset1 + 6) {
                  Parker_Velocity_Control_B.ia++;
                  Parker_Velocity_Control_B.X[Parker_Velocity_Control_B.b_ic] +=
                    Parker_Velocity_Control_B.U[Parker_Velocity_Control_B.ib - 1]
                    * Parker_Velocity_Control_B.V[Parker_Velocity_Control_B.ia];
                  Parker_Velocity_Control_B.b_ic++;
                }

                Parker_Velocity_Control_B.ar += 6;
                Parker_Velocity_Control_B.ib += 6;
              }
            }
          }
        }

        // MATLAB Function: '<S490>/Coriolis Function'
        // '<S492>:1:9' xdot   = [ x(7:12)
        // '<S492>:1:10'            q_ddot   ];
        memset(&Parker_Velocity_Control_B.InertiaS[0], 0, 12U * sizeof(real_T));
        Parker_Velocity_Control_B.t7_tmp = (Parker_Velocity_Control_B.t27 +
          Parker_Velocity_Control_B.t9_tmp * -cos
          (Parker_Velocity_Control_B.t15_tmp + 1.5707963267948966)) +
          Parker_Velocity_Control_B.t25;
        Parker_Velocity_Control_B.InertiaS[12] =
          Parker_Velocity_Control_B.t7_tmp + Parker_Velocity_Control_B.t7 * cos
          (Parker_Velocity_Control_B.t14_tmp);
        Parker_Velocity_Control_B.t15_tmp = (Parker_Velocity_Control_B.t23 +
          Parker_Velocity_Control_B.t9_tmp * -sin
          (Parker_Velocity_Control_B.t15_tmp + 1.5707963267948966)) +
          Parker_Velocity_Control_B.t51;
        Parker_Velocity_Control_B.InertiaS[13] =
          Parker_Velocity_Control_B.t15_tmp + Parker_Velocity_Control_B.t7 * sin
          (Parker_Velocity_Control_B.t14_tmp);
        Parker_Velocity_Control_B.t14_tmp = Parker_Velocity_Control_B.t6 +
          Parker_Velocity_Control_B.t4;
        Parker_Velocity_Control_B.t9_tmp = Parker_Velocity_Control_B.t14_tmp +
          Parker_Velocity_Control_B.t197;
        Parker_Velocity_Control_B.InertiaS[14] =
          ((Parker_Velocity_Control_B.t9_tmp - ((((Parker_Velocity_Control_B.t31
                + Parker_Velocity_Control_B.t33) +
               Parker_Velocity_Control_B.rtb_x_ddot_idx_1) +
              Parker_Velocity_Control_B.t135) + Parker_Velocity_Control_B.t117) *
            Parker_Velocity_Control_B.t127) - ((Parker_Velocity_Control_B.Sum6_d
             + Parker_Velocity_Control_B.t97) + Parker_Velocity_Control_B.t109) *
           Parker_Velocity_Control_B.t113) - Parker_Velocity_Control_B.t4_tmp *
          Parker_Velocity_Control_B.t106;
        Parker_Velocity_Control_B.Sum6_d = (Parker_Velocity_Control_B.t24 +
          Parker_Velocity_Control_B.t6) + Parker_Velocity_Control_B.t106 *
          Parker_Velocity_Control_B.t174;
        Parker_Velocity_Control_B.InertiaS[15] =
          ((((Parker_Velocity_Control_B.t30 +
              Parker_Velocity_Control_B.rtb_Sum_h_idx_0) +
             Parker_Velocity_Control_B.t34) + Parker_Velocity_Control_B.t110) +
           Parker_Velocity_Control_B.t114) * Parker_Velocity_Control_B.t127 +
          ((Parker_Velocity_Control_B.Sum6_d + Parker_Velocity_Control_B.t4) +
           Parker_Velocity_Control_B.t197);
        Parker_Velocity_Control_B.InertiaS[16] =
          (Parker_Velocity_Control_B.Sum6_d + Parker_Velocity_Control_B.t37) +
          Parker_Velocity_Control_B.t17;
        Parker_Velocity_Control_B.InertiaS[17] = (Parker_Velocity_Control_B.t24
          + Parker_Velocity_Control_B.t21) + Parker_Velocity_Control_B.t14;
        Parker_Velocity_Control_B.InertiaS[18] =
          Parker_Velocity_Control_B.t7_tmp;
        Parker_Velocity_Control_B.InertiaS[19] =
          Parker_Velocity_Control_B.t15_tmp;
        Parker_Velocity_Control_B.InertiaS[20] =
          (((Parker_Velocity_Control_B.t14_tmp + Parker_Velocity_Control_B.t35)
            + Parker_Velocity_Control_B.t197) + Parker_Velocity_Control_B.t45) -
          (((((((((Parker_Velocity_Control_B.t30 + Parker_Velocity_Control_B.t31)
                  + Parker_Velocity_Control_B.rtb_Sum_h_idx_0) +
                 Parker_Velocity_Control_B.t33) + Parker_Velocity_Control_B.t34)
               + Parker_Velocity_Control_B.rtb_x_ddot_idx_1) +
              Parker_Velocity_Control_B.t110) + Parker_Velocity_Control_B.t135)
            + Parker_Velocity_Control_B.t114) + Parker_Velocity_Control_B.t117) *
          Parker_Velocity_Control_B.t127;
        Parker_Velocity_Control_B.InertiaS[21] =
          Parker_Velocity_Control_B.t9_tmp;
        Parker_Velocity_Control_B.InertiaS[22] = (Parker_Velocity_Control_B.t6 +
          Parker_Velocity_Control_B.t37) + Parker_Velocity_Control_B.t17;
        Parker_Velocity_Control_B.InertiaS[23] = Parker_Velocity_Control_B.t21 +
          Parker_Velocity_Control_B.t14;
        Parker_Velocity_Control_B.InertiaS[24] = Parker_Velocity_Control_B.t27 +
          Parker_Velocity_Control_B.t25;
        Parker_Velocity_Control_B.InertiaS[25] = Parker_Velocity_Control_B.t23 +
          Parker_Velocity_Control_B.t51;
        Parker_Velocity_Control_B.InertiaS[26] = (((Parker_Velocity_Control_B.t6
          + Parker_Velocity_Control_B.t35) + Parker_Velocity_Control_B.t85) +
          Parker_Velocity_Control_B.t45) + Parker_Velocity_Control_B.t2;
        Parker_Velocity_Control_B.InertiaS[27] = (Parker_Velocity_Control_B.t6 +
          Parker_Velocity_Control_B.t85) + Parker_Velocity_Control_B.t2;
        Parker_Velocity_Control_B.InertiaS[28] =
          Parker_Velocity_Control_B.Sum6_l *
          Parker_Velocity_Control_B.rtb_Sum_h_idx_2 *
          -Parker_Velocity_Control_B.rtb_Sum_h_idx_1;
        Parker_Velocity_Control_B.InertiaS[29] =
          Parker_Velocity_Control_P.model_param[12] *
          Parker_Velocity_Control_B.Sum6_l * Parker_Velocity_Control_B.t75 *
          Parker_Velocity_Control_B.t41;
        Parker_Velocity_Control_B.InertiaS[30] = Parker_Velocity_Control_B.t8 *
          sin(Parker_Velocity_Control_B.t44);
        Parker_Velocity_Control_B.InertiaS[31] = -Parker_Velocity_Control_B.t8 *
          cos(Parker_Velocity_Control_B.t44);
        Parker_Velocity_Control_B.t7_tmp = Parker_Velocity_Control_B.t53 +
          Parker_Velocity_Control_B.t52;
        Parker_Velocity_Control_B.InertiaS[32] =
          Parker_Velocity_Control_B.t7_tmp -
          Parker_Velocity_Control_P.model_param[12] *
          Parker_Velocity_Control_B.rtb_TmpSignalConversionAtSFu_jw *
          Parker_Velocity_Control_B.rtb_TmpSignalConversionAtSFun_d *
          Parker_Velocity_Control_B.t113;
        Parker_Velocity_Control_B.InertiaS[33] =
          Parker_Velocity_Control_B.t7_tmp;
        Parker_Velocity_Control_B.InertiaS[34] = Parker_Velocity_Control_B.t53;
        Parker_Velocity_Control_B.InertiaS[35] = 0.0;

        // SignalConversion generated from: '<S492>/ SFunction ' incorporates:
        //   DataStoreRead: '<S490>/Data Store Read1'
        //   DataStoreRead: '<S490>/Data Store Read2'
        //   DataStoreRead: '<S490>/Data Store Read3'
        //   DataStoreRead: '<S490>/Data Store Read4'
        //   DataStoreRead: '<S490>/RED_Fx_Sat1'
        //   DataStoreRead: '<S490>/RED_Fy_Sat1'
        //   DataStoreRead: '<S490>/RED_Tz_Sat1'
        //   MATLAB Function: '<S490>/Dynamics Model'
        //   Sum: '<S490>/Sum1'

        Parker_Velocity_Control_B.X_f[0] = Parker_Velocity_Control_DW.RED_Fx_Sat;
        Parker_Velocity_Control_B.X_f[1] = Parker_Velocity_Control_DW.RED_Fy_Sat;
        Parker_Velocity_Control_B.X_f[2] = Parker_Velocity_Control_DW.RED_Tz_Sat
          + Parker_Velocity_Control_DW.RED_Tz_RW_Sat;
        Parker_Velocity_Control_B.X_f[3] =
          Parker_Velocity_Control_DW.RED_Tz_Shoulder;
        Parker_Velocity_Control_B.X_f[4] =
          Parker_Velocity_Control_DW.RED_Tz_Elbow;
        Parker_Velocity_Control_B.X_f[5] =
          Parker_Velocity_Control_DW.RED_Tz_Wrist;

        // MATLAB Function: '<S490>/Dynamics Model'
        Parker_Velocity_Control_B.dv2[0] = 0.0;
        Parker_Velocity_Control_B.dv2[1] = 0.0;
        Parker_Velocity_Control_B.dv2[2] = 0.0;
        Parker_Velocity_Control_B.dv2[3] = ((tanh
          (Parker_Velocity_Control_P.Gamma2 * rtb_Delay_0[9]) - tanh
          (Parker_Velocity_Control_P.Gamma3 * rtb_Delay_0[9])) *
          Parker_Velocity_Control_P.Gamma1 + tanh
          (Parker_Velocity_Control_P.Gamma5 * rtb_Delay_0[9]) *
          Parker_Velocity_Control_P.Gamma4) + Parker_Velocity_Control_P.Gamma6 *
          rtb_Delay_0[9];
        Parker_Velocity_Control_B.dv2[4] = ((tanh
          (Parker_Velocity_Control_P.Gamma2 * rtb_Delay_0[10]) - tanh
          (Parker_Velocity_Control_P.Gamma3 * rtb_Delay_0[10])) *
          Parker_Velocity_Control_P.Gamma1 + tanh
          (Parker_Velocity_Control_P.Gamma5 * rtb_Delay_0[10]) *
          Parker_Velocity_Control_P.Gamma4) + Parker_Velocity_Control_P.Gamma6 *
          rtb_Delay_0[10];
        Parker_Velocity_Control_B.dv2[5] = ((tanh
          (Parker_Velocity_Control_P.Gamma2 * rtb_Delay_0[11]) - tanh
          (Parker_Velocity_Control_P.Gamma3 * rtb_Delay_0[11])) *
          Parker_Velocity_Control_P.Gamma1 + tanh
          (Parker_Velocity_Control_P.Gamma5 * rtb_Delay_0[11]) *
          Parker_Velocity_Control_P.Gamma4) + Parker_Velocity_Control_P.Gamma6 *
          rtb_Delay_0[11];
        for (Parker_Velocity_Control_B.i = 0; Parker_Velocity_Control_B.i < 6;
             Parker_Velocity_Control_B.i++) {
          Parker_Velocity_Control_B.dv1[Parker_Velocity_Control_B.i] = 0.0;
          for (Parker_Velocity_Control_B.uElOffset1 = 0;
               Parker_Velocity_Control_B.uElOffset1 < 6;
               Parker_Velocity_Control_B.uElOffset1++) {
            Parker_Velocity_Control_B.dv1[Parker_Velocity_Control_B.i] +=
              Parker_Velocity_Control_B.InertiaS[6 *
              Parker_Velocity_Control_B.uElOffset1 + Parker_Velocity_Control_B.i]
              * rtb_Delay_0[Parker_Velocity_Control_B.uElOffset1 + 6];
          }

          Parker_Velocity_Control_B.s[Parker_Velocity_Control_B.i] =
            (Parker_Velocity_Control_B.X_f[Parker_Velocity_Control_B.i] -
             Parker_Velocity_Control_B.dv1[Parker_Velocity_Control_B.i]) -
            Parker_Velocity_Control_B.dv2[Parker_Velocity_Control_B.i];
        }

        for (Parker_Velocity_Control_B.i = 0; Parker_Velocity_Control_B.i < 6;
             Parker_Velocity_Control_B.i++) {
          Parker_Velocity_Control_B.X_f[Parker_Velocity_Control_B.i] = 0.0;
          for (Parker_Velocity_Control_B.uElOffset1 = 0;
               Parker_Velocity_Control_B.uElOffset1 < 6;
               Parker_Velocity_Control_B.uElOffset1++) {
            Parker_Velocity_Control_B.X_f[Parker_Velocity_Control_B.i] +=
              Parker_Velocity_Control_B.X[6 *
              Parker_Velocity_Control_B.uElOffset1 + Parker_Velocity_Control_B.i]
              * Parker_Velocity_Control_B.s[Parker_Velocity_Control_B.uElOffset1];
          }

          // DiscreteIntegrator: '<S490>/Acceleration  to Velocity'
          Parker_Velocity_Control_B.AccelerationtoVelocity_o[Parker_Velocity_Control_B.i]
            = rtb_Delay_0[Parker_Velocity_Control_B.i + 6] *
            Parker_Velocity_Control_P.AccelerationtoVelocity_gainval +
            Parker_Velocity_Control_DW.AccelerationtoVelocity_DSTATE_o[Parker_Velocity_Control_B.i];
          Parker_Velocity_Control_B.AccelerationtoVelocity_o[Parker_Velocity_Control_B.i
            + 6] =
            Parker_Velocity_Control_DW.AccelerationtoVelocity_DSTATE_o[Parker_Velocity_Control_B.i
            + 6] + Parker_Velocity_Control_P.AccelerationtoVelocity_gainval *
            Parker_Velocity_Control_B.X_f[Parker_Velocity_Control_B.i];
        }

        // DataStoreWrite: '<S490>/Data Store Write'
        Parker_Velocity_Control_DW.RED_Rz_Shoulder =
          Parker_Velocity_Control_B.AccelerationtoVelocity_o[3];

        // DataStoreWrite: '<S490>/Data Store Write1'
        Parker_Velocity_Control_DW.RED_Rz_Elbow =
          Parker_Velocity_Control_B.AccelerationtoVelocity_o[4];

        // DataStoreWrite: '<S490>/Data Store Write2'
        Parker_Velocity_Control_DW.RED_Rz_Wrist =
          Parker_Velocity_Control_B.AccelerationtoVelocity_o[5];

        // DataStoreWrite: '<S490>/Data Store Write6'
        Parker_Velocity_Control_DW.RED_RzD_Shoulder =
          Parker_Velocity_Control_B.AccelerationtoVelocity_o[9];

        // DataStoreWrite: '<S490>/Data Store Write7'
        Parker_Velocity_Control_DW.RED_RzD_Elbow =
          Parker_Velocity_Control_B.AccelerationtoVelocity_o[10];

        // DataStoreWrite: '<S490>/Data Store Write8'
        Parker_Velocity_Control_DW.RED_RzD_Wrist =
          Parker_Velocity_Control_B.AccelerationtoVelocity_o[11];

        // Delay: '<S495>/Delay1'
        if (Parker_Velocity_Control_DW.icLoad_nt != 0) {
          Parker_Velocity_Control_DW.Delay1_DSTATE_po =
            Parker_Velocity_Control_B.AccelerationtoVelocity_o[6];
        }

        // Sum: '<S495>/Sum6' incorporates:
        //   Delay: '<S495>/Delay1'

        Parker_Velocity_Control_B.Sum6_l =
          Parker_Velocity_Control_B.AccelerationtoVelocity_o[6] -
          Parker_Velocity_Control_DW.Delay1_DSTATE_po;

        // If: '<S495>/if we went through a "step"' incorporates:
        //   Inport: '<S497>/In1'

        if (Parker_Velocity_Control_B.Sum6_l != 0.0) {
          // Outputs for IfAction SubSystem: '<S495>/Hold this value' incorporates:
          //   ActionPort: '<S497>/Action Port'

          Parker_Velocity_Control_B.In1_h = Parker_Velocity_Control_B.Sum6_l;

          // End of Outputs for SubSystem: '<S495>/Hold this value'
        }

        // End of If: '<S495>/if we went through a "step"'

        // Sum: '<S490>/Sum2' incorporates:
        //   DataStoreWrite: '<S490>/RED_IMU_Ay1'
        //   Gain: '<S495>/divide by delta T'
        //   RandomNumber: '<S490>/Random Number2'

        Parker_Velocity_Control_DW.RED_IMU_Ax_I = 1.0 /
          Parker_Velocity_Control_P.baseRate * Parker_Velocity_Control_B.In1_h +
          Parker_Velocity_Control_DW.NextOutput_c2;

        // Delay: '<S494>/Delay1'
        if (Parker_Velocity_Control_DW.icLoad_el != 0) {
          Parker_Velocity_Control_DW.Delay1_DSTATE_kx =
            Parker_Velocity_Control_B.AccelerationtoVelocity_o[7];
        }

        // Sum: '<S494>/Sum6' incorporates:
        //   Delay: '<S494>/Delay1'

        Parker_Velocity_Control_B.Sum6_l =
          Parker_Velocity_Control_B.AccelerationtoVelocity_o[7] -
          Parker_Velocity_Control_DW.Delay1_DSTATE_kx;

        // If: '<S494>/if we went through a "step"' incorporates:
        //   Inport: '<S496>/In1'

        if (Parker_Velocity_Control_B.Sum6_l != 0.0) {
          // Outputs for IfAction SubSystem: '<S494>/Hold this value' incorporates:
          //   ActionPort: '<S496>/Action Port'

          Parker_Velocity_Control_B.In1_lg = Parker_Velocity_Control_B.Sum6_l;

          // End of Outputs for SubSystem: '<S494>/Hold this value'
        }

        // End of If: '<S494>/if we went through a "step"'

        // Sum: '<S490>/Sum3' incorporates:
        //   DataStoreWrite: '<S490>/RED_IMU_Ay_I1'
        //   Gain: '<S494>/divide by delta T'
        //   RandomNumber: '<S490>/Random Number1'

        Parker_Velocity_Control_DW.RED_IMU_Ay_I = 1.0 /
          Parker_Velocity_Control_P.baseRate * Parker_Velocity_Control_B.In1_lg
          + Parker_Velocity_Control_DW.NextOutput_lz;

        // Sum: '<S490>/Sum4' incorporates:
        //   DataStoreWrite: '<S490>/RED_IMU_R1'
        //   RandomNumber: '<S490>/Random Number3'

        Parker_Velocity_Control_DW.RED_IMU_R =
          Parker_Velocity_Control_B.AccelerationtoVelocity_o[8] +
          Parker_Velocity_Control_DW.NextOutput_dv;

        // DataStoreWrite: '<S490>/RED_Px1'
        Parker_Velocity_Control_DW.RED_Px =
          Parker_Velocity_Control_B.AccelerationtoVelocity_o[0];

        // DataStoreWrite: '<S490>/RED_Py1'
        Parker_Velocity_Control_DW.RED_Py =
          Parker_Velocity_Control_B.AccelerationtoVelocity_o[1];

        // DataStoreWrite: '<S490>/RED_Rz1'
        Parker_Velocity_Control_DW.RED_Rz =
          Parker_Velocity_Control_B.AccelerationtoVelocity_o[2];

        // DataStoreWrite: '<S490>/RED_RzD1'
        Parker_Velocity_Control_DW.RED_RzD =
          Parker_Velocity_Control_B.AccelerationtoVelocity_o[8];

        // DataStoreWrite: '<S490>/RED_Vx1'
        Parker_Velocity_Control_DW.RED_Vx =
          Parker_Velocity_Control_B.AccelerationtoVelocity_o[6];

        // DataStoreWrite: '<S490>/RED_Vy1'
        Parker_Velocity_Control_DW.RED_Vy =
          Parker_Velocity_Control_B.AccelerationtoVelocity_o[7];

        // Update for Delay: '<S490>/Delay'
        memcpy(&Parker_Velocity_Control_DW.Delay_DSTATE[0],
               &Parker_Velocity_Control_B.AccelerationtoVelocity_o[0], 12U *
               sizeof(real_T));

        // Update for DiscreteIntegrator: '<S490>/Acceleration  to Velocity' incorporates:
        //   Delay: '<S490>/Delay'

        memcpy(&Parker_Velocity_Control_DW.AccelerationtoVelocity_DSTATE_o[0],
               &Parker_Velocity_Control_B.AccelerationtoVelocity_o[0], 12U *
               sizeof(real_T));

        // Update for Delay: '<S495>/Delay1'
        Parker_Velocity_Control_DW.icLoad_nt = 0U;
        Parker_Velocity_Control_DW.Delay1_DSTATE_po =
          Parker_Velocity_Control_B.AccelerationtoVelocity_o[6];

        // Update for RandomNumber: '<S490>/Random Number2'
        Parker_Velocity_Control_DW.NextOutput_c2 =
          Park_rt_nrand_Upu32_Yd_f_pw_snf(&Parker_Velocity_Control_DW.RandSeed_o)
          * Parker_Velocity_Control_P.RandomNumber2_StdDev +
          Parker_Velocity_Control_P.RandomNumber2_Mean;

        // Update for Delay: '<S494>/Delay1'
        Parker_Velocity_Control_DW.icLoad_el = 0U;
        Parker_Velocity_Control_DW.Delay1_DSTATE_kx =
          Parker_Velocity_Control_B.AccelerationtoVelocity_o[7];

        // Update for RandomNumber: '<S490>/Random Number1'
        Parker_Velocity_Control_DW.NextOutput_lz =
          Park_rt_nrand_Upu32_Yd_f_pw_snf(&Parker_Velocity_Control_DW.RandSeed_p)
          * Parker_Velocity_Control_P.RandomNumber1_StdDev +
          Parker_Velocity_Control_P.RandomNumber1_Mean;

        // Update for RandomNumber: '<S490>/Random Number3'
        Parker_Velocity_Control_DW.NextOutput_dv =
          Park_rt_nrand_Upu32_Yd_f_pw_snf(&Parker_Velocity_Control_DW.RandSeed_h)
          * Parker_Velocity_Control_P.RandomNumber3_StdDev +
          Parker_Velocity_Control_P.RandomNumber3_Mean;

        // End of Outputs for SubSystem: '<S460>/RED + ARM'
      } else {
        // Outputs for IfAction SubSystem: '<S460>/RED Only' incorporates:
        //   ActionPort: '<S489>/Action Port'

        // DiscreteIntegrator: '<S499>/Acceleration  to Velocity' incorporates:
        //   DataStoreRead: '<S498>/RED_Fx_Sat'
        //   MATLAB Function: '<S499>/MATLAB Function'
        //   SignalConversion generated from: '<S505>/ SFunction '

        // MATLAB Function 'Simulate Plant Dynamics/RED and ARM Dynamics/RED Only/RED Dynamics/RED Dynamics Model/MATLAB Function': '<S505>:1' 
        // '<S505>:1:5' x_ddot     = zeros(3,1);
        // '<S505>:1:9' m_RED    = model_param(1);
        // '<S505>:1:10' I_RED    = model_param(2);
        // '<S505>:1:13' Fx        = control_inputs(1);
        // '<S505>:1:14' Fy        = control_inputs(2);
        // '<S505>:1:15' Tz        = control_inputs(3);
        // '<S505>:1:18' x_ddot(1) = Fx/m_RED;
        // '<S505>:1:19' x_ddot(2) = Fy/m_RED;
        // '<S505>:1:20' x_ddot(3) = Tz/I_RED;
        Parker_Velocity_Control_B.t30 = Parker_Velocity_Control_DW.RED_Fx_Sat /
          Parker_Velocity_Control_P.model_param[0] *
          Parker_Velocity_Control_P.AccelerationtoVelocity_gainva_k +
          Parker_Velocity_Control_DW.AccelerationtoVelocity_DSTATE_m[0];

        // DiscreteIntegrator: '<S499>/Velocity to Position'
        Parker_Velocity_Control_B.t31 =
          Parker_Velocity_Control_P.VelocitytoPosition_gainval *
          Parker_Velocity_Control_B.t30 +
          Parker_Velocity_Control_DW.VelocitytoPosition_DSTATE_f[0];

        // Sum: '<S499>/Sum' incorporates:
        //   RandomNumber: '<S499>/Random Number'

        Parker_Velocity_Control_B.rtb_Sum_h_idx_0 =
          Parker_Velocity_Control_B.t31 +
          Parker_Velocity_Control_DW.NextOutput_lo;
        Parker_Velocity_Control_B.Sum6_d = Parker_Velocity_Control_B.t30;
        Parker_Velocity_Control_B.rtb_TmpSignalConversionAtSFu_jw =
          Parker_Velocity_Control_B.t31;

        // DiscreteIntegrator: '<S499>/Acceleration  to Velocity' incorporates:
        //   DataStoreRead: '<S498>/RED_Fy_Sat'
        //   DiscreteIntegrator: '<S499>/Velocity to Position'
        //   MATLAB Function: '<S499>/MATLAB Function'
        //   SignalConversion generated from: '<S505>/ SFunction '

        Parker_Velocity_Control_B.t30 = Parker_Velocity_Control_DW.RED_Fy_Sat /
          Parker_Velocity_Control_P.model_param[0] *
          Parker_Velocity_Control_P.AccelerationtoVelocity_gainva_k +
          Parker_Velocity_Control_DW.AccelerationtoVelocity_DSTATE_m[1];

        // DiscreteIntegrator: '<S499>/Velocity to Position'
        Parker_Velocity_Control_B.t31 =
          Parker_Velocity_Control_P.VelocitytoPosition_gainval *
          Parker_Velocity_Control_B.t30 +
          Parker_Velocity_Control_DW.VelocitytoPosition_DSTATE_f[1];

        // Sum: '<S499>/Sum' incorporates:
        //   RandomNumber: '<S499>/Random Number'

        Parker_Velocity_Control_B.rtb_Sum_h_idx_1 =
          Parker_Velocity_Control_B.t31 +
          Parker_Velocity_Control_DW.NextOutput_lo;
        Parker_Velocity_Control_B.rtb_x_ddot_idx_1 =
          Parker_Velocity_Control_B.t30;
        Parker_Velocity_Control_B.rtb_TmpSignalConversionAtSFun_d =
          Parker_Velocity_Control_B.t31;

        // DiscreteIntegrator: '<S499>/Acceleration  to Velocity' incorporates:
        //   DataStoreRead: '<S498>/Data Store Read'
        //   DataStoreRead: '<S498>/RED_Tz_Sat'
        //   DiscreteIntegrator: '<S499>/Velocity to Position'
        //   MATLAB Function: '<S499>/MATLAB Function'
        //   Sum: '<S498>/Sum'

        Parker_Velocity_Control_B.t30 = (Parker_Velocity_Control_DW.RED_Tz_Sat +
          Parker_Velocity_Control_DW.RED_Tz_RW_Sat) /
          Parker_Velocity_Control_P.model_param[1] *
          Parker_Velocity_Control_P.AccelerationtoVelocity_gainva_k +
          Parker_Velocity_Control_DW.AccelerationtoVelocity_DSTATE_m[2];

        // DiscreteIntegrator: '<S499>/Velocity to Position'
        Parker_Velocity_Control_B.t31 =
          Parker_Velocity_Control_P.VelocitytoPosition_gainval *
          Parker_Velocity_Control_B.t30 +
          Parker_Velocity_Control_DW.VelocitytoPosition_DSTATE_f[2];

        // Sum: '<S499>/Sum' incorporates:
        //   RandomNumber: '<S499>/Random Number'

        Parker_Velocity_Control_B.rtb_Sum_h_idx_2 =
          Parker_Velocity_Control_B.t31 +
          Parker_Velocity_Control_DW.NextOutput_lo;

        // Delay: '<S501>/Delay1'
        if (Parker_Velocity_Control_DW.icLoad_ec != 0) {
          Parker_Velocity_Control_DW.Delay1_DSTATE_g =
            Parker_Velocity_Control_B.rtb_Sum_h_idx_0;
        }

        // Sum: '<S501>/Sum6' incorporates:
        //   Delay: '<S501>/Delay1'

        Parker_Velocity_Control_B.Sum6_l =
          Parker_Velocity_Control_B.rtb_Sum_h_idx_0 -
          Parker_Velocity_Control_DW.Delay1_DSTATE_g;

        // If: '<S501>/if we went through a "step"' incorporates:
        //   Inport: '<S507>/In1'

        if (Parker_Velocity_Control_B.Sum6_l != 0.0) {
          // Outputs for IfAction SubSystem: '<S501>/Hold this value' incorporates:
          //   ActionPort: '<S507>/Action Port'

          Parker_Velocity_Control_B.In1_ad = Parker_Velocity_Control_B.Sum6_l;

          // End of Outputs for SubSystem: '<S501>/Hold this value'
        }

        // End of If: '<S501>/if we went through a "step"'

        // Gain: '<S501>/divide by delta T' incorporates:
        //   DataStoreWrite: '<S498>/RED_Vx'

        Parker_Velocity_Control_DW.RED_Vx = 1.0 /
          Parker_Velocity_Control_P.baseRate * Parker_Velocity_Control_B.In1_ad;

        // Delay: '<S503>/Delay1' incorporates:
        //   DataStoreWrite: '<S498>/RED_Vx'

        if (Parker_Velocity_Control_DW.icLoad_by != 0) {
          Parker_Velocity_Control_DW.Delay1_DSTATE_m =
            Parker_Velocity_Control_DW.RED_Vx;
        }

        // Sum: '<S503>/Sum6' incorporates:
        //   DataStoreWrite: '<S498>/RED_Vx'
        //   Delay: '<S503>/Delay1'

        Parker_Velocity_Control_B.Sum6_l = Parker_Velocity_Control_DW.RED_Vx -
          Parker_Velocity_Control_DW.Delay1_DSTATE_m;

        // If: '<S503>/if we went through a "step"' incorporates:
        //   Inport: '<S509>/In1'

        if (Parker_Velocity_Control_B.Sum6_l != 0.0) {
          // Outputs for IfAction SubSystem: '<S503>/Hold this value' incorporates:
          //   ActionPort: '<S509>/Action Port'

          Parker_Velocity_Control_B.In1_pk = Parker_Velocity_Control_B.Sum6_l;

          // End of Outputs for SubSystem: '<S503>/Hold this value'
        }

        // End of If: '<S503>/if we went through a "step"'

        // Sum: '<S498>/Sum7' incorporates:
        //   DataStoreWrite: '<S498>/RED_IMU_Ay'
        //   Gain: '<S503>/divide by delta T'
        //   RandomNumber: '<S498>/Random Number7'

        Parker_Velocity_Control_DW.RED_IMU_Ax_I = 1.0 /
          Parker_Velocity_Control_P.baseRate * Parker_Velocity_Control_B.In1_pk
          + Parker_Velocity_Control_DW.NextOutput_de;

        // Delay: '<S500>/Delay1'
        if (Parker_Velocity_Control_DW.icLoad_oe != 0) {
          Parker_Velocity_Control_DW.Delay1_DSTATE_et =
            Parker_Velocity_Control_B.rtb_Sum_h_idx_1;
        }

        // Sum: '<S500>/Sum6' incorporates:
        //   Delay: '<S500>/Delay1'

        Parker_Velocity_Control_B.Sum6_l =
          Parker_Velocity_Control_B.rtb_Sum_h_idx_1 -
          Parker_Velocity_Control_DW.Delay1_DSTATE_et;

        // If: '<S500>/if we went through a "step"' incorporates:
        //   Inport: '<S506>/In1'

        if (Parker_Velocity_Control_B.Sum6_l != 0.0) {
          // Outputs for IfAction SubSystem: '<S500>/Hold this value' incorporates:
          //   ActionPort: '<S506>/Action Port'

          Parker_Velocity_Control_B.In1_kj = Parker_Velocity_Control_B.Sum6_l;

          // End of Outputs for SubSystem: '<S500>/Hold this value'
        }

        // End of If: '<S500>/if we went through a "step"'

        // Gain: '<S500>/divide by delta T' incorporates:
        //   DataStoreWrite: '<S498>/RED_Vy'

        Parker_Velocity_Control_DW.RED_Vy = 1.0 /
          Parker_Velocity_Control_P.baseRate * Parker_Velocity_Control_B.In1_kj;

        // Delay: '<S504>/Delay1' incorporates:
        //   DataStoreWrite: '<S498>/RED_Vy'

        if (Parker_Velocity_Control_DW.icLoad_mq != 0) {
          Parker_Velocity_Control_DW.Delay1_DSTATE_ln =
            Parker_Velocity_Control_DW.RED_Vy;
        }

        // Sum: '<S504>/Sum6' incorporates:
        //   DataStoreWrite: '<S498>/RED_Vy'
        //   Delay: '<S504>/Delay1'

        Parker_Velocity_Control_B.Sum6_l = Parker_Velocity_Control_DW.RED_Vy -
          Parker_Velocity_Control_DW.Delay1_DSTATE_ln;

        // If: '<S504>/if we went through a "step"' incorporates:
        //   Inport: '<S510>/In1'

        if (Parker_Velocity_Control_B.Sum6_l != 0.0) {
          // Outputs for IfAction SubSystem: '<S504>/Hold this value' incorporates:
          //   ActionPort: '<S510>/Action Port'

          Parker_Velocity_Control_B.In1_lk = Parker_Velocity_Control_B.Sum6_l;

          // End of Outputs for SubSystem: '<S504>/Hold this value'
        }

        // End of If: '<S504>/if we went through a "step"'

        // Sum: '<S498>/Sum8' incorporates:
        //   DataStoreWrite: '<S498>/RED_IMU_Ay_I'
        //   Gain: '<S504>/divide by delta T'
        //   RandomNumber: '<S498>/Random Number6'

        Parker_Velocity_Control_DW.RED_IMU_Ay_I = 1.0 /
          Parker_Velocity_Control_P.baseRate * Parker_Velocity_Control_B.In1_lk
          + Parker_Velocity_Control_DW.NextOutput_c;

        // Delay: '<S502>/Delay1'
        if (Parker_Velocity_Control_DW.icLoad_ov != 0) {
          Parker_Velocity_Control_DW.Delay1_DSTATE_hs =
            Parker_Velocity_Control_B.rtb_Sum_h_idx_2;
        }

        // Sum: '<S502>/Sum6' incorporates:
        //   Delay: '<S502>/Delay1'

        Parker_Velocity_Control_B.Sum6_l =
          Parker_Velocity_Control_B.rtb_Sum_h_idx_2 -
          Parker_Velocity_Control_DW.Delay1_DSTATE_hs;

        // If: '<S502>/if we went through a "step"' incorporates:
        //   Inport: '<S508>/In1'

        if (Parker_Velocity_Control_B.Sum6_l != 0.0) {
          // Outputs for IfAction SubSystem: '<S502>/Hold this value' incorporates:
          //   ActionPort: '<S508>/Action Port'

          Parker_Velocity_Control_B.In1_k = Parker_Velocity_Control_B.Sum6_l;

          // End of Outputs for SubSystem: '<S502>/Hold this value'
        }

        // End of If: '<S502>/if we went through a "step"'

        // Gain: '<S502>/divide by delta T' incorporates:
        //   DataStoreWrite: '<S498>/RED_RzD'

        Parker_Velocity_Control_DW.RED_RzD = 1.0 /
          Parker_Velocity_Control_P.baseRate * Parker_Velocity_Control_B.In1_k;

        // Sum: '<S498>/Sum9' incorporates:
        //   DataStoreWrite: '<S498>/RED_IMU_R'
        //   DataStoreWrite: '<S498>/RED_RzD'
        //   RandomNumber: '<S498>/Random Number8'

        Parker_Velocity_Control_DW.RED_IMU_R =
          Parker_Velocity_Control_DW.RED_RzD +
          Parker_Velocity_Control_DW.NextOutput_h;

        // DataStoreWrite: '<S498>/RED_Px'
        Parker_Velocity_Control_DW.RED_Px =
          Parker_Velocity_Control_B.rtb_Sum_h_idx_0;

        // DataStoreWrite: '<S498>/RED_Py'
        Parker_Velocity_Control_DW.RED_Py =
          Parker_Velocity_Control_B.rtb_Sum_h_idx_1;

        // DataStoreWrite: '<S498>/RED_Rz'
        Parker_Velocity_Control_DW.RED_Rz =
          Parker_Velocity_Control_B.rtb_Sum_h_idx_2;

        // Update for DiscreteIntegrator: '<S499>/Acceleration  to Velocity'
        Parker_Velocity_Control_DW.AccelerationtoVelocity_DSTATE_m[0] =
          Parker_Velocity_Control_B.Sum6_d;

        // Update for DiscreteIntegrator: '<S499>/Velocity to Position'
        Parker_Velocity_Control_DW.VelocitytoPosition_DSTATE_f[0] =
          Parker_Velocity_Control_B.rtb_TmpSignalConversionAtSFu_jw;

        // Update for DiscreteIntegrator: '<S499>/Acceleration  to Velocity'
        Parker_Velocity_Control_DW.AccelerationtoVelocity_DSTATE_m[1] =
          Parker_Velocity_Control_B.rtb_x_ddot_idx_1;

        // Update for DiscreteIntegrator: '<S499>/Velocity to Position'
        Parker_Velocity_Control_DW.VelocitytoPosition_DSTATE_f[1] =
          Parker_Velocity_Control_B.rtb_TmpSignalConversionAtSFun_d;

        // Update for DiscreteIntegrator: '<S499>/Acceleration  to Velocity'
        Parker_Velocity_Control_DW.AccelerationtoVelocity_DSTATE_m[2] =
          Parker_Velocity_Control_B.t30;

        // Update for DiscreteIntegrator: '<S499>/Velocity to Position'
        Parker_Velocity_Control_DW.VelocitytoPosition_DSTATE_f[2] =
          Parker_Velocity_Control_B.t31;

        // Update for RandomNumber: '<S499>/Random Number'
        Parker_Velocity_Control_DW.NextOutput_lo =
          Park_rt_nrand_Upu32_Yd_f_pw_snf
          (&Parker_Velocity_Control_DW.RandSeed_mt) * sqrt
          (Parker_Velocity_Control_P.noise_variance_RED) +
          Parker_Velocity_Control_P.RandomNumber_Mean;

        // Update for Delay: '<S501>/Delay1'
        Parker_Velocity_Control_DW.icLoad_ec = 0U;
        Parker_Velocity_Control_DW.Delay1_DSTATE_g =
          Parker_Velocity_Control_B.rtb_Sum_h_idx_0;

        // Update for Delay: '<S503>/Delay1' incorporates:
        //   DataStoreWrite: '<S498>/RED_Vx'

        Parker_Velocity_Control_DW.icLoad_by = 0U;
        Parker_Velocity_Control_DW.Delay1_DSTATE_m =
          Parker_Velocity_Control_DW.RED_Vx;

        // Update for RandomNumber: '<S498>/Random Number7'
        Parker_Velocity_Control_DW.NextOutput_de =
          Park_rt_nrand_Upu32_Yd_f_pw_snf
          (&Parker_Velocity_Control_DW.RandSeed_ka) *
          Parker_Velocity_Control_P.RandomNumber7_StdDev +
          Parker_Velocity_Control_P.RandomNumber7_Mean;

        // Update for Delay: '<S500>/Delay1'
        Parker_Velocity_Control_DW.icLoad_oe = 0U;
        Parker_Velocity_Control_DW.Delay1_DSTATE_et =
          Parker_Velocity_Control_B.rtb_Sum_h_idx_1;

        // Update for Delay: '<S504>/Delay1' incorporates:
        //   DataStoreWrite: '<S498>/RED_Vy'

        Parker_Velocity_Control_DW.icLoad_mq = 0U;
        Parker_Velocity_Control_DW.Delay1_DSTATE_ln =
          Parker_Velocity_Control_DW.RED_Vy;

        // Update for RandomNumber: '<S498>/Random Number6'
        Parker_Velocity_Control_DW.NextOutput_c =
          Park_rt_nrand_Upu32_Yd_f_pw_snf(&Parker_Velocity_Control_DW.RandSeed_g)
          * Parker_Velocity_Control_P.RandomNumber6_StdDev +
          Parker_Velocity_Control_P.RandomNumber6_Mean;

        // Update for Delay: '<S502>/Delay1'
        Parker_Velocity_Control_DW.icLoad_ov = 0U;
        Parker_Velocity_Control_DW.Delay1_DSTATE_hs =
          Parker_Velocity_Control_B.rtb_Sum_h_idx_2;

        // Update for RandomNumber: '<S498>/Random Number8'
        Parker_Velocity_Control_DW.NextOutput_h =
          Park_rt_nrand_Upu32_Yd_f_pw_snf(&Parker_Velocity_Control_DW.RandSeed_f)
          * Parker_Velocity_Control_P.RandomNumber8_StdDev +
          Parker_Velocity_Control_P.RandomNumber8_Mean;

        // End of Outputs for SubSystem: '<S460>/RED Only'
      }

      // End of If: '<S460>/If'

      // Update for DiscreteIntegrator: '<S461>/Acceleration  to Velocity'
      Parker_Velocity_Control_DW.AccelerationtoVelocity_DSTATE[0] =
        Parker_Velocity_Control_B.x_ddot_b[0];

      // Update for DiscreteIntegrator: '<S461>/Velocity to Position'
      Parker_Velocity_Control_DW.VelocitytoPosition_DSTATE[0] =
        Parker_Velocity_Control_B.VelocitytoPosition_idx_0;

      // Update for DiscreteIntegrator: '<S461>/Acceleration  to Velocity'
      Parker_Velocity_Control_DW.AccelerationtoVelocity_DSTATE[1] =
        Parker_Velocity_Control_B.x_ddot_b[1];

      // Update for DiscreteIntegrator: '<S461>/Velocity to Position'
      Parker_Velocity_Control_DW.VelocitytoPosition_DSTATE[1] =
        Parker_Velocity_Control_B.VelocitytoPosition_idx_1;

      // Update for DiscreteIntegrator: '<S461>/Acceleration  to Velocity'
      Parker_Velocity_Control_DW.AccelerationtoVelocity_DSTATE[2] =
        Parker_Velocity_Control_B.rtb_x_ddot_b_g;

      // Update for DiscreteIntegrator: '<S461>/Velocity to Position'
      Parker_Velocity_Control_DW.VelocitytoPosition_DSTATE[2] =
        Parker_Velocity_Control_B.VelocitytoPosition;

      // Update for RandomNumber: '<S461>/Random Number'
      Parker_Velocity_Control_DW.NextOutput = Park_rt_nrand_Upu32_Yd_f_pw_snf
        (&Parker_Velocity_Control_DW.RandSeed) * sqrt
        (Parker_Velocity_Control_P.noise_variance_BLACK) +
        Parker_Velocity_Control_P.RandomNumber_Mean_n;

      // Update for Delay: '<S466>/Delay1'
      Parker_Velocity_Control_DW.icLoad_bb = 0U;
      Parker_Velocity_Control_DW.Delay1_DSTATE_p =
        Parker_Velocity_Control_B.rtb_Sum_idx_0;

      // Update for Delay: '<S464>/Delay1' incorporates:
      //   DataStoreWrite: '<S457>/BLACK_Vx'

      Parker_Velocity_Control_DW.icLoad_p = 0U;
      Parker_Velocity_Control_DW.Delay1_DSTATE_ja =
        Parker_Velocity_Control_DW.BLACK_Vx;

      // Update for RandomNumber: '<S457>/Random Number1'
      Parker_Velocity_Control_DW.NextOutput_p = Park_rt_nrand_Upu32_Yd_f_pw_snf(
        &Parker_Velocity_Control_DW.RandSeed_m) *
        Parker_Velocity_Control_P.RandomNumber1_StdDev_o +
        Parker_Velocity_Control_P.RandomNumber1_Mean_i;

      // Update for Delay: '<S462>/Delay1'
      Parker_Velocity_Control_DW.icLoad_lv = 0U;
      Parker_Velocity_Control_DW.Delay1_DSTATE_d4 =
        Parker_Velocity_Control_B.rtb_Sum_idx_1;

      // Update for Delay: '<S465>/Delay1' incorporates:
      //   DataStoreWrite: '<S457>/BLACK_Vy'

      Parker_Velocity_Control_DW.icLoad_hu = 0U;
      Parker_Velocity_Control_DW.Delay1_DSTATE_j3 =
        Parker_Velocity_Control_DW.BLACK_Vy;

      // Update for RandomNumber: '<S457>/Random Number'
      Parker_Velocity_Control_DW.NextOutput_o = Park_rt_nrand_Upu32_Yd_f_pw_snf(
        &Parker_Velocity_Control_DW.RandSeed_d) *
        Parker_Velocity_Control_P.RandomNumber_StdDev +
        Parker_Velocity_Control_P.RandomNumber_Mean_e;

      // Update for Delay: '<S463>/Delay1'
      Parker_Velocity_Control_DW.icLoad_m = 0U;
      Parker_Velocity_Control_DW.Delay1_DSTATE_km =
        Parker_Velocity_Control_B.rtb_Sum_idx_2;

      // Update for RandomNumber: '<S457>/Random Number2'
      Parker_Velocity_Control_DW.NextOutput_j = Park_rt_nrand_Upu32_Yd_f_pw_snf(
        &Parker_Velocity_Control_DW.RandSeed_k) *
        Parker_Velocity_Control_P.RandomNumber2_StdDev_p +
        Parker_Velocity_Control_P.RandomNumber2_Mean_c;

      // Update for DiscreteIntegrator: '<S473>/Acceleration  to Velocity'
      Parker_Velocity_Control_DW.AccelerationtoVelocity_DSTATE_b[0] =
        Parker_Velocity_Control_B.rtb_x_ddot_i_idx_0;

      // Update for DiscreteIntegrator: '<S473>/Velocity to Position'
      Parker_Velocity_Control_DW.VelocitytoPosition_DSTATE_a[0] =
        Parker_Velocity_Control_B.rtb_TmpSignalConversionAtSFun_p;

      // Update for DiscreteIntegrator: '<S473>/Acceleration  to Velocity'
      Parker_Velocity_Control_DW.AccelerationtoVelocity_DSTATE_b[1] =
        Parker_Velocity_Control_B.rtb_x_ddot_i_idx_1;

      // Update for DiscreteIntegrator: '<S473>/Velocity to Position'
      Parker_Velocity_Control_DW.VelocitytoPosition_DSTATE_a[1] =
        Parker_Velocity_Control_B.rtb_TmpSignalConversionAtSFun_l;

      // Update for DiscreteIntegrator: '<S473>/Acceleration  to Velocity'
      Parker_Velocity_Control_DW.AccelerationtoVelocity_DSTATE_b[2] =
        Parker_Velocity_Control_B.rtb_x_ddot_i_m;

      // Update for DiscreteIntegrator: '<S473>/Velocity to Position'
      Parker_Velocity_Control_DW.VelocitytoPosition_DSTATE_a[2] =
        Parker_Velocity_Control_B.rtb_TmpSignalConversionAtSFu_nz;

      // Update for RandomNumber: '<S473>/Random Number'
      Parker_Velocity_Control_DW.NextOutput_g = Park_rt_nrand_Upu32_Yd_f_pw_snf(
        &Parker_Velocity_Control_DW.RandSeed_d3) * sqrt
        (Parker_Velocity_Control_P.noise_variance_BLUE) +
        Parker_Velocity_Control_P.RandomNumber_Mean_b;

      // Update for Delay: '<S478>/Delay1'
      Parker_Velocity_Control_DW.icLoad_dc = 0U;
      Parker_Velocity_Control_DW.Delay1_DSTATE_e =
        Parker_Velocity_Control_B.rtb_Sum_l_idx_0;

      // Update for Delay: '<S476>/Delay1' incorporates:
      //   DataStoreWrite: '<S458>/BLUE_Vx'

      Parker_Velocity_Control_DW.icLoad_ln = 0U;
      Parker_Velocity_Control_DW.Delay1_DSTATE_fi =
        Parker_Velocity_Control_DW.BLUE_Vx;

      // Update for RandomNumber: '<S458>/Random Number4'
      Park_rt_nrand_Upu32_Yd_f_pw_snf(&Parker_Velocity_Control_DW.RandSeed_dc);

      // Update for Delay: '<S474>/Delay1'
      Parker_Velocity_Control_DW.icLoad_oc = 0U;
      Parker_Velocity_Control_DW.Delay1_DSTATE_b =
        Parker_Velocity_Control_B.rtb_Sum_l_idx_1;

      // Update for Delay: '<S477>/Delay1' incorporates:
      //   DataStoreWrite: '<S458>/BLUE_Vy'

      Parker_Velocity_Control_DW.icLoad_l1 = 0U;
      Parker_Velocity_Control_DW.Delay1_DSTATE_fq =
        Parker_Velocity_Control_DW.BLUE_Vy;

      // Update for RandomNumber: '<S458>/Random Number3'
      Park_rt_nrand_Upu32_Yd_f_pw_snf(&Parker_Velocity_Control_DW.RandSeed_j);

      // Update for Delay: '<S475>/Delay1'
      Parker_Velocity_Control_DW.icLoad_lz = 0U;
      Parker_Velocity_Control_DW.Delay1_DSTATE_od =
        Parker_Velocity_Control_B.rtb_Sum_l_idx_2;

      // Update for RandomNumber: '<S458>/Random Number5'
      Parker_Velocity_Control_DW.NextOutput_k = Park_rt_nrand_Upu32_Yd_f_pw_snf(
        &Parker_Velocity_Control_DW.RandSeed_c) *
        Parker_Velocity_Control_P.RandomNumber5_StdDev +
        Parker_Velocity_Control_P.RandomNumber5_Mean;

      // Update for DiscreteIntegrator: '<S459>/Discrete-Time Integrator' incorporates:
      //   DataStoreRead: '<S459>/Data Store Read3'

      Parker_Velocity_Control_DW.DiscreteTimeIntegrator_IC_LOADI = 0U;
      Parker_Velocity_Control_DW.DiscreteTimeIntegrator_DSTATE +=
        Parker_Velocity_Control_P.DiscreteTimeIntegrator_gainval *
        Parker_Velocity_Control_B.Subtract_o;
      Parker_Velocity_Control_DW.DiscreteTimeIntegrator_PrevRese =
        static_cast<int8_T>(Parker_Velocity_Control_B.rtb_Compare_j);

      // Update for DiscreteIntegrator: '<S459>/Discrete-Time Integrator1' incorporates:
      //   DataStoreWrite: '<S459>/RED_Px1'

      Parker_Velocity_Control_DW.DiscreteTimeIntegrator1_IC_LOAD = 0U;
      Parker_Velocity_Control_DW.DiscreteTimeIntegrator1_DSTATE =
        Parker_Velocity_Control_DW.BLACK_IMU_Psi;
      Parker_Velocity_Control_DW.DiscreteTimeIntegrator1_PrevRes =
        static_cast<int8_T>(Parker_Velocity_Control_B.rtb_Compare_n_m);

      // Update for DiscreteIntegrator: '<S459>/Discrete-Time Integrator2' incorporates:
      //   DataStoreWrite: '<S459>/RED_Px2'

      Parker_Velocity_Control_DW.DiscreteTimeIntegrator2_IC_LOAD = 0U;
      Parker_Velocity_Control_DW.DiscreteTimeIntegrator2_DSTATE =
        Parker_Velocity_Control_DW.RED_IMU_Psi;
      Parker_Velocity_Control_DW.DiscreteTimeIntegrator2_PrevRes =
        static_cast<int8_T>(Parker_Velocity_Control_B.rtb_Compare_f_m);

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

    Parker_Velocity_Control_B.TmpSignalConversionAtToWork[0] =
      Parker_Velocity_Control_DW.Univ_Time;
    Parker_Velocity_Control_B.TmpSignalConversionAtToWork[1] =
      Parker_Velocity_Control_DW.RED_Fx_Sat;
    Parker_Velocity_Control_B.TmpSignalConversionAtToWork[2] =
      Parker_Velocity_Control_DW.RED_Fy_Sat;
    Parker_Velocity_Control_B.TmpSignalConversionAtToWork[3] =
      Parker_Velocity_Control_DW.RED_Tz_Sat;
    Parker_Velocity_Control_B.TmpSignalConversionAtToWork[4] =
      Parker_Velocity_Control_DW.RED_Px;
    Parker_Velocity_Control_B.TmpSignalConversionAtToWork[5] =
      Parker_Velocity_Control_DW.RED_Py;
    Parker_Velocity_Control_B.TmpSignalConversionAtToWork[6] =
      Parker_Velocity_Control_DW.RED_Rz;
    Parker_Velocity_Control_B.TmpSignalConversionAtToWork[7] =
      Parker_Velocity_Control_DW.RED_Vx;
    Parker_Velocity_Control_B.TmpSignalConversionAtToWork[8] =
      Parker_Velocity_Control_DW.RED_Vy;
    Parker_Velocity_Control_B.TmpSignalConversionAtToWork[9] =
      Parker_Velocity_Control_DW.RED_RzD;
    Parker_Velocity_Control_B.TmpSignalConversionAtToWork[10] =
      Parker_Velocity_Control_DW.RED_AHRS_Q;
    Parker_Velocity_Control_B.TmpSignalConversionAtToWork[11] =
      Parker_Velocity_Control_DW.RED_AHRS_P;
    Parker_Velocity_Control_B.TmpSignalConversionAtToWork[12] =
      Parker_Velocity_Control_DW.RED_AHRS_R;
    Parker_Velocity_Control_B.TmpSignalConversionAtToWork[13] =
      Parker_Velocity_Control_DW.RED_IMU_Ax_b;
    Parker_Velocity_Control_B.TmpSignalConversionAtToWork[14] =
      Parker_Velocity_Control_DW.RED_IMU_Ay_b;
    Parker_Velocity_Control_B.TmpSignalConversionAtToWork[15] =
      Parker_Velocity_Control_DW.RED_IMU_Az_b;
    Parker_Velocity_Control_B.TmpSignalConversionAtToWork[16] =
      Parker_Velocity_Control_DW.RED_IMU_Ax_I;
    Parker_Velocity_Control_B.TmpSignalConversionAtToWork[17] =
      Parker_Velocity_Control_DW.RED_IMU_Ay_I;
    Parker_Velocity_Control_B.TmpSignalConversionAtToWork[18] =
      Parker_Velocity_Control_DW.RED_Ax;
    Parker_Velocity_Control_B.TmpSignalConversionAtToWork[19] =
      Parker_Velocity_Control_DW.RED_Ay;
    Parker_Velocity_Control_B.TmpSignalConversionAtToWork[20] =
      Parker_Velocity_Control_DW.RED_RzDD;
    Parker_Velocity_Control_B.TmpSignalConversionAtToWork[21] =
      Parker_Velocity_Control_DW.BLACK_Fx_Sat;
    Parker_Velocity_Control_B.TmpSignalConversionAtToWork[22] =
      Parker_Velocity_Control_DW.BLACK_Fy_Sat;
    Parker_Velocity_Control_B.TmpSignalConversionAtToWork[23] =
      Parker_Velocity_Control_DW.BLACK_Tz_Sat;
    Parker_Velocity_Control_B.TmpSignalConversionAtToWork[24] =
      Parker_Velocity_Control_DW.BLACK_Px;
    Parker_Velocity_Control_B.TmpSignalConversionAtToWork[25] =
      Parker_Velocity_Control_DW.BLACK_Py;
    Parker_Velocity_Control_B.TmpSignalConversionAtToWork[26] =
      Parker_Velocity_Control_DW.BLACK_Rz;
    Parker_Velocity_Control_B.TmpSignalConversionAtToWork[27] =
      Parker_Velocity_Control_DW.BLACK_Vx;
    Parker_Velocity_Control_B.TmpSignalConversionAtToWork[28] =
      Parker_Velocity_Control_DW.BLACK_Vy;
    Parker_Velocity_Control_B.TmpSignalConversionAtToWork[29] =
      Parker_Velocity_Control_DW.BLACK_RzD;
    Parker_Velocity_Control_B.TmpSignalConversionAtToWork[30] =
      Parker_Velocity_Control_DW.BLACK_AHRS_Q;
    Parker_Velocity_Control_B.TmpSignalConversionAtToWork[31] =
      Parker_Velocity_Control_DW.BLACK_AHRS_P;
    Parker_Velocity_Control_B.TmpSignalConversionAtToWork[32] =
      Parker_Velocity_Control_DW.BLACK_AHRS_R;
    Parker_Velocity_Control_B.TmpSignalConversionAtToWork[33] =
      Parker_Velocity_Control_DW.BLACK_IMU_Ax_b;
    Parker_Velocity_Control_B.TmpSignalConversionAtToWork[34] =
      Parker_Velocity_Control_DW.BLACK_IMU_Ay_b;
    Parker_Velocity_Control_B.TmpSignalConversionAtToWork[35] =
      Parker_Velocity_Control_DW.BLACK_IMU_Az_b;
    Parker_Velocity_Control_B.TmpSignalConversionAtToWork[36] =
      Parker_Velocity_Control_DW.BLACK_IMU_Ax_I;
    Parker_Velocity_Control_B.TmpSignalConversionAtToWork[37] =
      Parker_Velocity_Control_DW.BLACK_IMU_Ay_I;
    Parker_Velocity_Control_B.TmpSignalConversionAtToWork[38] =
      Parker_Velocity_Control_DW.BLACK_Ax;
    Parker_Velocity_Control_B.TmpSignalConversionAtToWork[39] =
      Parker_Velocity_Control_DW.BLACK_Ay;
    Parker_Velocity_Control_B.TmpSignalConversionAtToWork[40] =
      Parker_Velocity_Control_DW.BLACK_RzDD;
    Parker_Velocity_Control_B.TmpSignalConversionAtToWork[41] =
      Parker_Velocity_Control_DW.BLUE_Fx_Sat;
    Parker_Velocity_Control_B.TmpSignalConversionAtToWork[42] =
      Parker_Velocity_Control_DW.BLUE_Fy_Sat;
    Parker_Velocity_Control_B.TmpSignalConversionAtToWork[43] =
      Parker_Velocity_Control_DW.BLUE_Tz_Sat;
    Parker_Velocity_Control_B.TmpSignalConversionAtToWork[44] =
      Parker_Velocity_Control_DW.BLUE_Px;
    Parker_Velocity_Control_B.TmpSignalConversionAtToWork[45] =
      Parker_Velocity_Control_DW.BLUE_Py;
    Parker_Velocity_Control_B.TmpSignalConversionAtToWork[46] =
      Parker_Velocity_Control_DW.BLUE_Rz;
    Parker_Velocity_Control_B.TmpSignalConversionAtToWork[47] =
      Parker_Velocity_Control_DW.BLUE_Vx;
    Parker_Velocity_Control_B.TmpSignalConversionAtToWork[48] =
      Parker_Velocity_Control_DW.BLUE_Vy;
    Parker_Velocity_Control_B.TmpSignalConversionAtToWork[49] =
      Parker_Velocity_Control_DW.BLUE_RzD;
    Parker_Velocity_Control_B.TmpSignalConversionAtToWork[50] =
      Parker_Velocity_Control_DW.RED_IMU_Q;
    Parker_Velocity_Control_B.TmpSignalConversionAtToWork[51] =
      Parker_Velocity_Control_DW.RED_IMU_P;
    Parker_Velocity_Control_B.TmpSignalConversionAtToWork[52] =
      Parker_Velocity_Control_DW.RED_IMU_R;
    Parker_Velocity_Control_B.TmpSignalConversionAtToWork[53] =
      Parker_Velocity_Control_DW.BLACK_IMU_Q;
    Parker_Velocity_Control_B.TmpSignalConversionAtToWork[54] =
      Parker_Velocity_Control_DW.BLACK_IMU_P;
    Parker_Velocity_Control_B.TmpSignalConversionAtToWork[55] =
      Parker_Velocity_Control_DW.BLACK_IMU_R;
    Parker_Velocity_Control_B.TmpSignalConversionAtToWork[56] =
      Parker_Velocity_Control_DW.RED_IMU_Psi;
    Parker_Velocity_Control_B.TmpSignalConversionAtToWork[57] =
      Parker_Velocity_Control_DW.BLACK_IMU_Psi;
    Parker_Velocity_Control_B.TmpSignalConversionAtToWork[58] =
      Parker_Velocity_Control_DW.ARM_Elbow_Px;
    Parker_Velocity_Control_B.TmpSignalConversionAtToWork[59] =
      Parker_Velocity_Control_DW.ARM_Elbow_Py;
    Parker_Velocity_Control_B.TmpSignalConversionAtToWork[60] =
      Parker_Velocity_Control_DW.ARM_Wrist_Px;
    Parker_Velocity_Control_B.TmpSignalConversionAtToWork[61] =
      Parker_Velocity_Control_DW.ARM_Wrist_Py;
    Parker_Velocity_Control_B.TmpSignalConversionAtToWork[62] =
      Parker_Velocity_Control_DW.ARM_EndEff_Px;
    Parker_Velocity_Control_B.TmpSignalConversionAtToWork[63] =
      Parker_Velocity_Control_DW.ARM_EndEff_Py;
    Parker_Velocity_Control_B.TmpSignalConversionAtToWork[64] =
      Parker_Velocity_Control_DW.RED_Rz_Shoulder;
    Parker_Velocity_Control_B.TmpSignalConversionAtToWork[65] =
      Parker_Velocity_Control_DW.RED_Rz_Elbow;
    Parker_Velocity_Control_B.TmpSignalConversionAtToWork[66] =
      Parker_Velocity_Control_DW.RED_Rz_Wrist;
    Parker_Velocity_Control_B.TmpSignalConversionAtToWork[67] =
      Parker_Velocity_Control_DW.RED_RzD_Shoulder;
    Parker_Velocity_Control_B.TmpSignalConversionAtToWork[68] =
      Parker_Velocity_Control_DW.RED_RzD_Elbow;
    Parker_Velocity_Control_B.TmpSignalConversionAtToWork[69] =
      Parker_Velocity_Control_DW.RED_RzD_Wrist;
    Parker_Velocity_Control_B.TmpSignalConversionAtToWork[70] =
      Parker_Velocity_Control_DW.RED_Tz_Shoulder;
    Parker_Velocity_Control_B.TmpSignalConversionAtToWork[71] =
      Parker_Velocity_Control_DW.RED_Tz_Elbow;
    Parker_Velocity_Control_B.TmpSignalConversionAtToWork[72] =
      Parker_Velocity_Control_DW.RED_Tz_Wrist;

    // ToWorkspace: '<S1>/To Workspace'
    rt_UpdateLogVar((LogVar *)(LogVar*)
                    (Parker_Velocity_Control_DW.ToWorkspace_PWORK.LoggedData),
                    &Parker_Velocity_Control_B.TmpSignalConversionAtToWork[0], 0);

    // If: '<S2>/This IF block determines whether or not to run the exp code' incorporates:
    //   Constant: '<S22>/Constant'

    if (Parker_Velocity_Control_P.simMode == 0.0) {
      // Outputs for IfAction SubSystem: '<S2>/Change Behavior' incorporates:
      //   ActionPort: '<S21>/Action Port'

      // MATLABSystem: '<S23>/Digital Write' incorporates:
      //   DataStoreRead: '<S21>/Data Store Read'

      Parker_Velocity_Control_B.t7_tmp = rt_roundd_snf
        (Parker_Velocity_Control_DW.Magnet_State);
      if (Parker_Velocity_Control_B.t7_tmp < 256.0) {
        if (Parker_Velocity_Control_B.t7_tmp >= 0.0) {
          Parker_Velocity_Control_B.u = static_cast<uint8_T>
            (Parker_Velocity_Control_B.t7_tmp);
        } else {
          Parker_Velocity_Control_B.u = 0U;
        }
      } else {
        Parker_Velocity_Control_B.u = MAX_uint8_T;
      }

      MW_gpioWrite(10U, Parker_Velocity_Control_B.u);

      // End of MATLABSystem: '<S23>/Digital Write'

      // MATLABSystem: '<S24>/Digital Write' incorporates:
      //   DataStoreRead: '<S21>/Data Store Read1'

      Parker_Velocity_Control_B.t7_tmp = rt_roundd_snf
        (Parker_Velocity_Control_DW.Float_State);
      if (Parker_Velocity_Control_B.t7_tmp < 256.0) {
        if (Parker_Velocity_Control_B.t7_tmp >= 0.0) {
          Parker_Velocity_Control_B.u = static_cast<uint8_T>
            (Parker_Velocity_Control_B.t7_tmp);
        } else {
          Parker_Velocity_Control_B.u = 0U;
        }
      } else {
        Parker_Velocity_Control_B.u = MAX_uint8_T;
      }

      MW_gpioWrite(26U, Parker_Velocity_Control_B.u);

      // End of MATLABSystem: '<S24>/Digital Write'
      // End of Outputs for SubSystem: '<S2>/Change Behavior'
    }

    // End of If: '<S2>/This IF block determines whether or not to run the exp code' 

    // If: '<S4>/This IF block determines whether or not to run the BLACK sim//exp' incorporates:
    //   Constant: '<S32>/Constant'
    //   DataStoreRead: '<S29>/BLACK_Fx'
    //   DataStoreRead: '<S29>/BLACK_Fy'
    //   DataStoreRead: '<S4>/Data Store Read'
    //   Product: '<S35>/Rotate F_I to F_b'
    //   SignalConversion generated from: '<S35>/Rotate F_I to F_b'

    if ((Parker_Velocity_Control_DW.WhoAmI == 2.0) ||
        (Parker_Velocity_Control_P.simMode == 1.0)) {
      // Outputs for IfAction SubSystem: '<S4>/Change BLACK Behavior' incorporates:
      //   ActionPort: '<S29>/Action Port'

      // MATLAB Function: '<S41>/Create Rotation Matrix' incorporates:
      //   DataStoreRead: '<S34>/BLACK_Rz'

      Parker_CreateRotationMatrix(Parker_Velocity_Control_DW.BLACK_Rz,
        &Parker_Velocity_Control_B.sf_CreateRotationMatrix);

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
      memset(&Parker_Velocity_Control_B.Mat2[0], 0, sizeof(real_T) << 6U);

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
      for (Parker_Velocity_Control_B.uElOffset1 = 0;
           Parker_Velocity_Control_B.uElOffset1 < 8;
           Parker_Velocity_Control_B.uElOffset1++) {
        Parker_Velocity_Control_B.Mat2[Parker_Velocity_Control_B.uElOffset1 +
          (Parker_Velocity_Control_B.uElOffset1 << 3)] =
          Parker_Velocity_Control_P.F_thrusters_BLACK[Parker_Velocity_Control_B.uElOffset1];

        // MATLAB Function: '<S34>/MATLAB Function'
        Parker_Velocity_Control_B.rtb_Product_h_p[Parker_Velocity_Control_B.uElOffset1]
          =
          Parker_Velocity_Control_P.F_thrusters_BLACK[Parker_Velocity_Control_B.uElOffset1]
          / 2.0;
      }

      // MATLAB Function: '<S34>/MATLAB Function'
      memset(&Parker_Velocity_Control_B.Mat2_m[0], 0, sizeof(real_T) << 6U);

      // '<S36>:1:27' H    = Mat1*Mat2;
      for (Parker_Velocity_Control_B.uElOffset1 = 0;
           Parker_Velocity_Control_B.uElOffset1 < 8;
           Parker_Velocity_Control_B.uElOffset1++) {
        Parker_Velocity_Control_B.Mat2_m[Parker_Velocity_Control_B.uElOffset1 +
          (Parker_Velocity_Control_B.uElOffset1 << 3)] =
          Parker_Velocity_Control_B.rtb_Product_h_p[Parker_Velocity_Control_B.uElOffset1];
        Parker_Velocity_Control_B.b[3 * Parker_Velocity_Control_B.uElOffset1] =
          b[Parker_Velocity_Control_B.uElOffset1];
        Parker_Velocity_Control_B.b[3 * Parker_Velocity_Control_B.uElOffset1 + 1]
          = c[Parker_Velocity_Control_B.uElOffset1];
        Parker_Velocity_Control_B.b[3 * Parker_Velocity_Control_B.uElOffset1 + 2]
          =
          Parker_Velocity_Control_P.thruster_dist2CG_BLACK[Parker_Velocity_Control_B.uElOffset1]
          / 1000.0;
      }

      for (Parker_Velocity_Control_B.i = 0; Parker_Velocity_Control_B.i < 8;
           Parker_Velocity_Control_B.i++) {
        for (Parker_Velocity_Control_B.uElOffset1 = 0;
             Parker_Velocity_Control_B.uElOffset1 < 3;
             Parker_Velocity_Control_B.uElOffset1++) {
          Parker_Velocity_Control_B.ntIdx0 =
            Parker_Velocity_Control_B.uElOffset1 + 3 *
            Parker_Velocity_Control_B.i;
          Parker_Velocity_Control_B.H_hz[Parker_Velocity_Control_B.ntIdx0] = 0.0;
          for (Parker_Velocity_Control_B.vcol = 0;
               Parker_Velocity_Control_B.vcol < 8;
               Parker_Velocity_Control_B.vcol++) {
            Parker_Velocity_Control_B.H_hz[Parker_Velocity_Control_B.ntIdx0] +=
              Parker_Velocity_Control_B.b[3 * Parker_Velocity_Control_B.vcol +
              Parker_Velocity_Control_B.uElOffset1] *
              Parker_Velocity_Control_B.Mat2_m[(Parker_Velocity_Control_B.i << 3)
              + Parker_Velocity_Control_B.vcol];
          }
        }
      }

      // PermuteDimensions: '<S42>/transpose'
      Parker_Velocity_Control_B.i = 0;
      Parker_Velocity_Control_B.uElOffset1 = 0;
      for (Parker_Velocity_Control_B.vcol = 0; Parker_Velocity_Control_B.vcol <
           3; Parker_Velocity_Control_B.vcol++) {
        Parker_Velocity_Control_B.ar = Parker_Velocity_Control_B.uElOffset1;
        for (Parker_Velocity_Control_B.ntIdx0 = 0;
             Parker_Velocity_Control_B.ntIdx0 < 8;
             Parker_Velocity_Control_B.ntIdx0++) {
          Parker_Velocity_Control_B.Product2_j[Parker_Velocity_Control_B.i] =
            Parker_Velocity_Control_B.H_hz[Parker_Velocity_Control_B.ar];
          Parker_Velocity_Control_B.i++;
          Parker_Velocity_Control_B.ar += 3;
        }

        Parker_Velocity_Control_B.uElOffset1++;
      }

      // End of PermuteDimensions: '<S42>/transpose'

      // Product: '<S42>/Product'
      for (Parker_Velocity_Control_B.i = 0; Parker_Velocity_Control_B.i < 3;
           Parker_Velocity_Control_B.i++) {
        for (Parker_Velocity_Control_B.uElOffset1 = 0;
             Parker_Velocity_Control_B.uElOffset1 < 3;
             Parker_Velocity_Control_B.uElOffset1++) {
          Parker_Velocity_Control_B.ntIdx0 =
            Parker_Velocity_Control_B.uElOffset1 + 3 *
            Parker_Velocity_Control_B.i;
          Parker_Velocity_Control_B.rtb_H_hz_k[Parker_Velocity_Control_B.ntIdx0]
            = 0.0;
          for (Parker_Velocity_Control_B.vcol = 0;
               Parker_Velocity_Control_B.vcol < 8;
               Parker_Velocity_Control_B.vcol++) {
            Parker_Velocity_Control_B.rtb_H_hz_k[Parker_Velocity_Control_B.ntIdx0]
              += Parker_Velocity_Control_B.H_hz[3 *
              Parker_Velocity_Control_B.vcol +
              Parker_Velocity_Control_B.uElOffset1] *
              Parker_Velocity_Control_B.Product2_j[(Parker_Velocity_Control_B.i <<
              3) + Parker_Velocity_Control_B.vcol];
          }
        }
      }

      // Product: '<S42>/Product1' incorporates:
      //   Product: '<S42>/Product'

      Parker_Velocity__rt_invd3x3_snf(Parker_Velocity_Control_B.rtb_H_hz_k,
        Parker_Velocity_Control_B.dv);

      // Product: '<S42>/Product2'
      for (Parker_Velocity_Control_B.i = 0; Parker_Velocity_Control_B.i < 3;
           Parker_Velocity_Control_B.i++) {
        for (Parker_Velocity_Control_B.uElOffset1 = 0;
             Parker_Velocity_Control_B.uElOffset1 < 8;
             Parker_Velocity_Control_B.uElOffset1++) {
          Parker_Velocity_Control_B.ntIdx0 =
            Parker_Velocity_Control_B.uElOffset1 + (Parker_Velocity_Control_B.i <<
            3);
          Parker_Velocity_Control_B.transpose_f[Parker_Velocity_Control_B.ntIdx0]
            = 0.0;
          Parker_Velocity_Control_B.transpose_f[Parker_Velocity_Control_B.ntIdx0]
            += Parker_Velocity_Control_B.dv[3 * Parker_Velocity_Control_B.i] *
            Parker_Velocity_Control_B.Product2_j[Parker_Velocity_Control_B.uElOffset1];
          Parker_Velocity_Control_B.transpose_f[Parker_Velocity_Control_B.ntIdx0]
            += Parker_Velocity_Control_B.dv[3 * Parker_Velocity_Control_B.i + 1]
            * Parker_Velocity_Control_B.Product2_j[Parker_Velocity_Control_B.uElOffset1
            + 8];
          Parker_Velocity_Control_B.transpose_f[Parker_Velocity_Control_B.ntIdx0]
            += Parker_Velocity_Control_B.dv[3 * Parker_Velocity_Control_B.i + 2]
            * Parker_Velocity_Control_B.Product2_j[Parker_Velocity_Control_B.uElOffset1
            + 16];
        }
      }

      // End of Product: '<S42>/Product2'

      // MATLAB Function: '<S35>/Create Rotation Matrix' incorporates:
      //   DataStoreRead: '<S29>/BLACK_Rz'

      Park_CreateRotationMatrix_h(Parker_Velocity_Control_DW.BLACK_Rz,
        &Parker_Velocity_Control_B.sf_CreateRotationMatrix_h);
      Parker_Velocity_Control_B.Subtract_o =
        Parker_Velocity_Control_B.sf_CreateRotationMatrix_h.C_bI[0] *
        Parker_Velocity_Control_DW.BLACK_Fx +
        Parker_Velocity_Control_B.sf_CreateRotationMatrix_h.C_bI[2] *
        Parker_Velocity_Control_DW.BLACK_Fy;

      // Product: '<S35>/Rotate F_I to F_b' incorporates:
      //   DataStoreRead: '<S29>/BLACK_Fx'
      //   DataStoreRead: '<S29>/BLACK_Fy'
      //   SignalConversion generated from: '<S35>/Rotate F_I to F_b'

      Parker_Velocity_Control_B.rtb_Sum_idx_0 =
        Parker_Velocity_Control_B.sf_CreateRotationMatrix_h.C_bI[1] *
        Parker_Velocity_Control_DW.BLACK_Fx +
        Parker_Velocity_Control_B.sf_CreateRotationMatrix_h.C_bI[3] *
        Parker_Velocity_Control_DW.BLACK_Fy;

      // SignalConversion generated from: '<S34>/Product' incorporates:
      //   DataStoreRead: '<S29>/BLACK_Tz'

      Parker_Velocity_Control_B.x_ddot_b[0] =
        Parker_Velocity_Control_B.Subtract_o;
      Parker_Velocity_Control_B.x_ddot_b[1] =
        Parker_Velocity_Control_B.rtb_Sum_idx_0;
      Parker_Velocity_Control_B.x_ddot_b[2] =
        Parker_Velocity_Control_DW.BLACK_Tz;

      // Product: '<S34>/Product' incorporates:
      //   DataStoreRead: '<S29>/BLACK_Tz'
      //   SignalConversion generated from: '<S34>/Product'

      for (Parker_Velocity_Control_B.i = 0; Parker_Velocity_Control_B.i < 8;
           Parker_Velocity_Control_B.i++) {
        Parker_Velocity_Control_B.rtb_Product_h_p[Parker_Velocity_Control_B.i] =
          Parker_Velocity_Control_B.transpose_f[Parker_Velocity_Control_B.i + 16]
          * Parker_Velocity_Control_DW.BLACK_Tz +
          (Parker_Velocity_Control_B.transpose_f[Parker_Velocity_Control_B.i + 8]
           * Parker_Velocity_Control_B.rtb_Sum_idx_0 +
           Parker_Velocity_Control_B.transpose_f[Parker_Velocity_Control_B.i] *
           Parker_Velocity_Control_B.Subtract_o);
      }

      // End of Product: '<S34>/Product'

      // MATLAB Function: '<S34>/MATLAB Function10'
      Parker_Vel_MATLABFunction10(Parker_Velocity_Control_B.rtb_Product_h_p,
        &Parker_Velocity_Control_B.sf_MATLABFunction10);

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
      Parker_Velocity_Control_B.rtb_Sum_idx_0 =
        Parker_Velocity_Control_P.F_Black_X_nominal *
        Parker_Velocity_Control_B.sf_MATLABFunction10.thrust_decay_factor / 2.0;
      Parker_Velocity_Control_B.rtb_Product_h_p[0] =
        Parker_Velocity_Control_B.rtb_Sum_idx_0;
      Parker_Velocity_Control_B.rtb_Product_h_p[1] =
        Parker_Velocity_Control_B.rtb_Sum_idx_0;
      Parker_Velocity_Control_B.VelocitytoPosition_idx_0 =
        Parker_Velocity_Control_P.F_Black_Y_nominal *
        Parker_Velocity_Control_B.sf_MATLABFunction10.thrust_decay_factor / 2.0;
      Parker_Velocity_Control_B.rtb_Product_h_p[2] =
        Parker_Velocity_Control_B.VelocitytoPosition_idx_0;
      Parker_Velocity_Control_B.rtb_Product_h_p[3] =
        Parker_Velocity_Control_B.VelocitytoPosition_idx_0;
      Parker_Velocity_Control_B.rtb_Product_h_p[4] =
        Parker_Velocity_Control_B.rtb_Sum_idx_0;
      Parker_Velocity_Control_B.rtb_Product_h_p[5] =
        Parker_Velocity_Control_B.rtb_Sum_idx_0;
      Parker_Velocity_Control_B.rtb_Product_h_p[6] =
        Parker_Velocity_Control_B.VelocitytoPosition_idx_0;
      Parker_Velocity_Control_B.rtb_Product_h_p[7] =
        Parker_Velocity_Control_B.VelocitytoPosition_idx_0;
      memset(&Parker_Velocity_Control_B.Mat2_m[0], 0, sizeof(real_T) << 6U);

      // '<S39>:1:41' H    = Mat1*Mat2;
      for (Parker_Velocity_Control_B.uElOffset1 = 0;
           Parker_Velocity_Control_B.uElOffset1 < 8;
           Parker_Velocity_Control_B.uElOffset1++) {
        Parker_Velocity_Control_B.Mat2_m[Parker_Velocity_Control_B.uElOffset1 +
          (Parker_Velocity_Control_B.uElOffset1 << 3)] =
          Parker_Velocity_Control_B.rtb_Product_h_p[Parker_Velocity_Control_B.uElOffset1];
        Parker_Velocity_Control_B.b[3 * Parker_Velocity_Control_B.uElOffset1] =
          b[Parker_Velocity_Control_B.uElOffset1];
        Parker_Velocity_Control_B.b[3 * Parker_Velocity_Control_B.uElOffset1 + 1]
          = c[Parker_Velocity_Control_B.uElOffset1];
      }

      Parker_Velocity_Control_B.b[2] =
        Parker_Velocity_Control_P.thruster_dist2CG_BLACK[0] / 1000.0;
      Parker_Velocity_Control_B.b[5] =
        Parker_Velocity_Control_P.thruster_dist2CG_BLACK[1] / 1000.0;
      Parker_Velocity_Control_B.b[8] =
        Parker_Velocity_Control_P.thruster_dist2CG_BLACK[2] / 1000.0;
      Parker_Velocity_Control_B.b[11] =
        Parker_Velocity_Control_P.thruster_dist2CG_BLACK[3] / 1000.0;
      Parker_Velocity_Control_B.b[14] =
        Parker_Velocity_Control_P.thruster_dist2CG_BLACK[4] / 1000.0;
      Parker_Velocity_Control_B.b[17] =
        Parker_Velocity_Control_P.thruster_dist2CG_BLACK[5] / 1000.0;
      Parker_Velocity_Control_B.b[20] =
        Parker_Velocity_Control_P.thruster_dist2CG_BLACK[6] / 1000.0;
      Parker_Velocity_Control_B.b[23] =
        Parker_Velocity_Control_P.thruster_dist2CG_BLACK[7] / 1000.0;
      for (Parker_Velocity_Control_B.i = 0; Parker_Velocity_Control_B.i < 8;
           Parker_Velocity_Control_B.i++) {
        for (Parker_Velocity_Control_B.uElOffset1 = 0;
             Parker_Velocity_Control_B.uElOffset1 < 3;
             Parker_Velocity_Control_B.uElOffset1++) {
          Parker_Velocity_Control_B.ntIdx0 =
            Parker_Velocity_Control_B.uElOffset1 + 3 *
            Parker_Velocity_Control_B.i;
          Parker_Velocity_Control_B.H_hz[Parker_Velocity_Control_B.ntIdx0] = 0.0;
          for (Parker_Velocity_Control_B.vcol = 0;
               Parker_Velocity_Control_B.vcol < 8;
               Parker_Velocity_Control_B.vcol++) {
            Parker_Velocity_Control_B.H_hz[Parker_Velocity_Control_B.ntIdx0] +=
              Parker_Velocity_Control_B.b[3 * Parker_Velocity_Control_B.vcol +
              Parker_Velocity_Control_B.uElOffset1] *
              Parker_Velocity_Control_B.Mat2_m[(Parker_Velocity_Control_B.i << 3)
              + Parker_Velocity_Control_B.vcol];
          }
        }
      }

      // End of MATLAB Function: '<S34>/MATLAB Function11'

      // PermuteDimensions: '<S43>/transpose'
      Parker_Velocity_Control_B.i = 0;
      Parker_Velocity_Control_B.uElOffset1 = 0;
      for (Parker_Velocity_Control_B.vcol = 0; Parker_Velocity_Control_B.vcol <
           3; Parker_Velocity_Control_B.vcol++) {
        Parker_Velocity_Control_B.ar = Parker_Velocity_Control_B.uElOffset1;
        for (Parker_Velocity_Control_B.ntIdx0 = 0;
             Parker_Velocity_Control_B.ntIdx0 < 8;
             Parker_Velocity_Control_B.ntIdx0++) {
          Parker_Velocity_Control_B.transpose_f[Parker_Velocity_Control_B.i] =
            Parker_Velocity_Control_B.H_hz[Parker_Velocity_Control_B.ar];
          Parker_Velocity_Control_B.i++;
          Parker_Velocity_Control_B.ar += 3;
        }

        Parker_Velocity_Control_B.uElOffset1++;
      }

      // End of PermuteDimensions: '<S43>/transpose'

      // Product: '<S43>/Product'
      for (Parker_Velocity_Control_B.i = 0; Parker_Velocity_Control_B.i < 3;
           Parker_Velocity_Control_B.i++) {
        for (Parker_Velocity_Control_B.uElOffset1 = 0;
             Parker_Velocity_Control_B.uElOffset1 < 3;
             Parker_Velocity_Control_B.uElOffset1++) {
          Parker_Velocity_Control_B.ntIdx0 =
            Parker_Velocity_Control_B.uElOffset1 + 3 *
            Parker_Velocity_Control_B.i;
          Parker_Velocity_Control_B.rtb_H_hz_k[Parker_Velocity_Control_B.ntIdx0]
            = 0.0;
          for (Parker_Velocity_Control_B.vcol = 0;
               Parker_Velocity_Control_B.vcol < 8;
               Parker_Velocity_Control_B.vcol++) {
            Parker_Velocity_Control_B.rtb_H_hz_k[Parker_Velocity_Control_B.ntIdx0]
              += Parker_Velocity_Control_B.H_hz[3 *
              Parker_Velocity_Control_B.vcol +
              Parker_Velocity_Control_B.uElOffset1] *
              Parker_Velocity_Control_B.transpose_f[(Parker_Velocity_Control_B.i
              << 3) + Parker_Velocity_Control_B.vcol];
          }
        }
      }

      // Product: '<S43>/Product1' incorporates:
      //   Product: '<S43>/Product'

      Parker_Velocity__rt_invd3x3_snf(Parker_Velocity_Control_B.rtb_H_hz_k,
        Parker_Velocity_Control_B.dv);
      for (Parker_Velocity_Control_B.i = 0; Parker_Velocity_Control_B.i < 8;
           Parker_Velocity_Control_B.i++) {
        // Product: '<S34>/Product5'
        Parker_Velocity_Control_B.rtb_transpose_f_c[Parker_Velocity_Control_B.i]
          = 0.0;
        for (Parker_Velocity_Control_B.uElOffset1 = 0;
             Parker_Velocity_Control_B.uElOffset1 < 3;
             Parker_Velocity_Control_B.uElOffset1++) {
          // Product: '<S43>/Product2' incorporates:
          //   Product: '<S34>/Product5'

          Parker_Velocity_Control_B.ntIdx0 = Parker_Velocity_Control_B.i +
            (Parker_Velocity_Control_B.uElOffset1 << 3);
          Parker_Velocity_Control_B.b[Parker_Velocity_Control_B.ntIdx0] = 0.0;
          Parker_Velocity_Control_B.b[Parker_Velocity_Control_B.ntIdx0] +=
            Parker_Velocity_Control_B.dv[3 *
            Parker_Velocity_Control_B.uElOffset1] *
            Parker_Velocity_Control_B.transpose_f[Parker_Velocity_Control_B.i];
          Parker_Velocity_Control_B.b[Parker_Velocity_Control_B.ntIdx0] +=
            Parker_Velocity_Control_B.dv[3 *
            Parker_Velocity_Control_B.uElOffset1 + 1] *
            Parker_Velocity_Control_B.transpose_f[Parker_Velocity_Control_B.i +
            8];
          Parker_Velocity_Control_B.b[Parker_Velocity_Control_B.ntIdx0] +=
            Parker_Velocity_Control_B.dv[3 *
            Parker_Velocity_Control_B.uElOffset1 + 2] *
            Parker_Velocity_Control_B.transpose_f[Parker_Velocity_Control_B.i +
            16];
          Parker_Velocity_Control_B.rtb_transpose_f_c[Parker_Velocity_Control_B.i]
            += Parker_Velocity_Control_B.b[Parker_Velocity_Control_B.ntIdx0] *
            Parker_Velocity_Control_B.x_ddot_b[Parker_Velocity_Control_B.uElOffset1];
        }

        // Saturate: '<S34>/Remove Negatives' incorporates:
        //   Product: '<S34>/Product5'
        //   Product: '<S43>/Product2'

        if (Parker_Velocity_Control_B.rtb_transpose_f_c[Parker_Velocity_Control_B.i]
            > Parker_Velocity_Control_P.RemoveNegatives_UpperSat) {
          Parker_Velocity_Control_B.rtb_Product_h_p[Parker_Velocity_Control_B.i]
            = Parker_Velocity_Control_P.RemoveNegatives_UpperSat;
        } else if
            (Parker_Velocity_Control_B.rtb_transpose_f_c[Parker_Velocity_Control_B.i]
             < Parker_Velocity_Control_P.RemoveNegatives_LowerSat) {
          Parker_Velocity_Control_B.rtb_Product_h_p[Parker_Velocity_Control_B.i]
            = Parker_Velocity_Control_P.RemoveNegatives_LowerSat;
        } else {
          Parker_Velocity_Control_B.rtb_Product_h_p[Parker_Velocity_Control_B.i]
            =
            Parker_Velocity_Control_B.rtb_transpose_f_c[Parker_Velocity_Control_B.i];
        }

        // End of Saturate: '<S34>/Remove Negatives'
      }

      // MATLAB Function: '<S34>/MATLAB Function2'
      Parker_Velo_MATLABFunction2(Parker_Velocity_Control_B.rtb_Product_h_p,
        &Parker_Velocity_Control_B.sf_MATLABFunction2);

      // MATLAB Function: '<S34>/MATLAB Function1'
      for (Parker_Velocity_Control_B.i = 0; Parker_Velocity_Control_B.i < 8;
           Parker_Velocity_Control_B.i++) {
        Parker_Velocity_Control_B.b[3 * Parker_Velocity_Control_B.i] =
          b[Parker_Velocity_Control_B.i];
        Parker_Velocity_Control_B.b[3 * Parker_Velocity_Control_B.i + 1] =
          c[Parker_Velocity_Control_B.i];
        Parker_Velocity_Control_B.b[3 * Parker_Velocity_Control_B.i + 2] =
          Parker_Velocity_Control_P.thruster_dist2CG_BLACK[Parker_Velocity_Control_B.i]
          / 1000.0;
      }

      for (Parker_Velocity_Control_B.i = 0; Parker_Velocity_Control_B.i < 8;
           Parker_Velocity_Control_B.i++) {
        for (Parker_Velocity_Control_B.uElOffset1 = 0;
             Parker_Velocity_Control_B.uElOffset1 < 3;
             Parker_Velocity_Control_B.uElOffset1++) {
          Parker_Velocity_Control_B.ntIdx0 =
            Parker_Velocity_Control_B.uElOffset1 + 3 *
            Parker_Velocity_Control_B.i;
          Parker_Velocity_Control_B.H_hz[Parker_Velocity_Control_B.ntIdx0] = 0.0;
          for (Parker_Velocity_Control_B.vcol = 0;
               Parker_Velocity_Control_B.vcol < 8;
               Parker_Velocity_Control_B.vcol++) {
            Parker_Velocity_Control_B.H_hz[Parker_Velocity_Control_B.ntIdx0] +=
              Parker_Velocity_Control_B.b[3 * Parker_Velocity_Control_B.vcol +
              Parker_Velocity_Control_B.uElOffset1] *
              Parker_Velocity_Control_B.Mat2[(Parker_Velocity_Control_B.i << 3)
              + Parker_Velocity_Control_B.vcol];
          }
        }
      }

      // Product: '<S34>/Product1'
      for (Parker_Velocity_Control_B.i = 0; Parker_Velocity_Control_B.i < 3;
           Parker_Velocity_Control_B.i++) {
        Parker_Velocity_Control_B.x_ddot_b[Parker_Velocity_Control_B.i] = 0.0;
        for (Parker_Velocity_Control_B.uElOffset1 = 0;
             Parker_Velocity_Control_B.uElOffset1 < 8;
             Parker_Velocity_Control_B.uElOffset1++) {
          Parker_Velocity_Control_B.x_ddot_b[Parker_Velocity_Control_B.i] +=
            Parker_Velocity_Control_B.H_hz[3 *
            Parker_Velocity_Control_B.uElOffset1 + Parker_Velocity_Control_B.i] *
            Parker_Velocity_Control_B.sf_MATLABFunction2.ThrustPer_Final[Parker_Velocity_Control_B.uElOffset1];
        }
      }

      // End of Product: '<S34>/Product1'

      // DataStoreWrite: '<S34>/BLACK_Fx_Sat' incorporates:
      //   Product: '<S41>/Rotate F_b to F_I'

      Parker_Velocity_Control_DW.BLACK_Fx_Sat =
        Parker_Velocity_Control_B.sf_CreateRotationMatrix.C_Ib[0] *
        Parker_Velocity_Control_B.x_ddot_b[0] +
        Parker_Velocity_Control_B.sf_CreateRotationMatrix.C_Ib[2] *
        Parker_Velocity_Control_B.x_ddot_b[1];

      // DataStoreWrite: '<S34>/BLACK_Fy_Sat' incorporates:
      //   Product: '<S41>/Rotate F_b to F_I'

      Parker_Velocity_Control_DW.BLACK_Fy_Sat =
        Parker_Velocity_Control_B.sf_CreateRotationMatrix.C_Ib[1] *
        Parker_Velocity_Control_B.x_ddot_b[0] +
        Parker_Velocity_Control_B.sf_CreateRotationMatrix.C_Ib[3] *
        Parker_Velocity_Control_B.x_ddot_b[1];

      // DataStoreWrite: '<S34>/BLACK_Tz_Sat'
      Parker_Velocity_Control_DW.BLACK_Tz_Sat =
        Parker_Velocity_Control_B.x_ddot_b[2];

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

    if ((Parker_Velocity_Control_DW.WhoAmI == 3.0) ||
        (Parker_Velocity_Control_P.simMode == 1.0)) {
      // Outputs for IfAction SubSystem: '<S4>/Change BLUE Behavior' incorporates:
      //   ActionPort: '<S30>/Action Port'

      // MATLAB Function: '<S53>/Create Rotation Matrix' incorporates:
      //   DataStoreRead: '<S46>/BLUE_Rz'

      Parker_CreateRotationMatrix(Parker_Velocity_Control_DW.BLUE_Rz,
        &Parker_Velocity_Control_B.sf_CreateRotationMatrix_i);

      // MATLAB Function: '<S46>/MATLAB Function1'
      Parker_Veloc_MATLABFunction
        (&Parker_Velocity_Control_B.sf_MATLABFunction1_b);

      // MATLAB Function: '<S46>/MATLAB Function'
      Parker_Veloc_MATLABFunction(&Parker_Velocity_Control_B.sf_MATLABFunction_i);

      // PermuteDimensions: '<S54>/transpose'
      Parker_Velocity_Control_B.i = 0;
      Parker_Velocity_Control_B.uElOffset1 = 0;
      for (Parker_Velocity_Control_B.vcol = 0; Parker_Velocity_Control_B.vcol <
           3; Parker_Velocity_Control_B.vcol++) {
        Parker_Velocity_Control_B.ar = Parker_Velocity_Control_B.uElOffset1;
        for (Parker_Velocity_Control_B.ntIdx0 = 0;
             Parker_Velocity_Control_B.ntIdx0 < 8;
             Parker_Velocity_Control_B.ntIdx0++) {
          Parker_Velocity_Control_B.Product2_a[Parker_Velocity_Control_B.i] =
            Parker_Velocity_Control_B.sf_MATLABFunction_i.H[Parker_Velocity_Control_B.ar];
          Parker_Velocity_Control_B.i++;
          Parker_Velocity_Control_B.ar += 3;
        }

        Parker_Velocity_Control_B.uElOffset1++;
      }

      // End of PermuteDimensions: '<S54>/transpose'

      // Product: '<S54>/Product'
      for (Parker_Velocity_Control_B.i = 0; Parker_Velocity_Control_B.i < 3;
           Parker_Velocity_Control_B.i++) {
        for (Parker_Velocity_Control_B.uElOffset1 = 0;
             Parker_Velocity_Control_B.uElOffset1 < 3;
             Parker_Velocity_Control_B.uElOffset1++) {
          Parker_Velocity_Control_B.vcol = Parker_Velocity_Control_B.uElOffset1
            + 3 * Parker_Velocity_Control_B.i;
          Parker_Velocity_Control_B.dv[Parker_Velocity_Control_B.vcol] = 0.0;
          for (Parker_Velocity_Control_B.ntIdx0 = 0;
               Parker_Velocity_Control_B.ntIdx0 < 8;
               Parker_Velocity_Control_B.ntIdx0++) {
            Parker_Velocity_Control_B.dv[Parker_Velocity_Control_B.vcol] +=
              Parker_Velocity_Control_B.sf_MATLABFunction_i.H[3 *
              Parker_Velocity_Control_B.ntIdx0 +
              Parker_Velocity_Control_B.uElOffset1] *
              Parker_Velocity_Control_B.Product2_a[(Parker_Velocity_Control_B.i <<
              3) + Parker_Velocity_Control_B.ntIdx0];
          }
        }
      }

      // Product: '<S54>/Product1' incorporates:
      //   Product: '<S54>/Product'

      Parker_Velocity__rt_invd3x3_snf(Parker_Velocity_Control_B.dv,
        Parker_Velocity_Control_B.rtb_H_hz_k);

      // Product: '<S54>/Product2'
      for (Parker_Velocity_Control_B.i = 0; Parker_Velocity_Control_B.i < 3;
           Parker_Velocity_Control_B.i++) {
        for (Parker_Velocity_Control_B.uElOffset1 = 0;
             Parker_Velocity_Control_B.uElOffset1 < 8;
             Parker_Velocity_Control_B.uElOffset1++) {
          Parker_Velocity_Control_B.ntIdx0 =
            Parker_Velocity_Control_B.uElOffset1 + (Parker_Velocity_Control_B.i <<
            3);
          Parker_Velocity_Control_B.transpose_f[Parker_Velocity_Control_B.ntIdx0]
            = 0.0;
          Parker_Velocity_Control_B.transpose_f[Parker_Velocity_Control_B.ntIdx0]
            += Parker_Velocity_Control_B.rtb_H_hz_k[3 *
            Parker_Velocity_Control_B.i] *
            Parker_Velocity_Control_B.Product2_a[Parker_Velocity_Control_B.uElOffset1];
          Parker_Velocity_Control_B.transpose_f[Parker_Velocity_Control_B.ntIdx0]
            += Parker_Velocity_Control_B.rtb_H_hz_k[3 *
            Parker_Velocity_Control_B.i + 1] *
            Parker_Velocity_Control_B.Product2_a[Parker_Velocity_Control_B.uElOffset1
            + 8];
          Parker_Velocity_Control_B.transpose_f[Parker_Velocity_Control_B.ntIdx0]
            += Parker_Velocity_Control_B.rtb_H_hz_k[3 *
            Parker_Velocity_Control_B.i + 2] *
            Parker_Velocity_Control_B.Product2_a[Parker_Velocity_Control_B.uElOffset1
            + 16];
        }
      }

      // End of Product: '<S54>/Product2'

      // MATLAB Function: '<S47>/Create Rotation Matrix' incorporates:
      //   DataStoreRead: '<S30>/BLUE_Rz'

      Park_CreateRotationMatrix_h(Parker_Velocity_Control_DW.BLUE_Rz,
        &Parker_Velocity_Control_B.sf_CreateRotationMatrix_f);
      Parker_Velocity_Control_B.Subtract_o =
        Parker_Velocity_Control_B.sf_CreateRotationMatrix_f.C_bI[0] *
        Parker_Velocity_Control_DW.BLUE_Fx +
        Parker_Velocity_Control_B.sf_CreateRotationMatrix_f.C_bI[2] *
        Parker_Velocity_Control_DW.BLUE_Fy;

      // Product: '<S47>/Rotate F_I to F_b' incorporates:
      //   DataStoreRead: '<S30>/BLUE_Fx'
      //   DataStoreRead: '<S30>/BLUE_Fy'
      //   SignalConversion generated from: '<S47>/Rotate F_I to F_b'

      Parker_Velocity_Control_B.rtb_Sum_idx_0 =
        Parker_Velocity_Control_B.sf_CreateRotationMatrix_f.C_bI[1] *
        Parker_Velocity_Control_DW.BLUE_Fx +
        Parker_Velocity_Control_B.sf_CreateRotationMatrix_f.C_bI[3] *
        Parker_Velocity_Control_DW.BLUE_Fy;

      // SignalConversion generated from: '<S46>/Product' incorporates:
      //   DataStoreRead: '<S30>/BLUE_Tz'

      Parker_Velocity_Control_B.x_ddot_b[0] =
        Parker_Velocity_Control_B.Subtract_o;
      Parker_Velocity_Control_B.x_ddot_b[1] =
        Parker_Velocity_Control_B.rtb_Sum_idx_0;
      Parker_Velocity_Control_B.x_ddot_b[2] = Parker_Velocity_Control_DW.BLUE_Tz;

      // Product: '<S46>/Product' incorporates:
      //   DataStoreRead: '<S30>/BLUE_Tz'
      //   SignalConversion generated from: '<S46>/Product'

      for (Parker_Velocity_Control_B.i = 0; Parker_Velocity_Control_B.i < 8;
           Parker_Velocity_Control_B.i++) {
        Parker_Velocity_Control_B.rtb_Product_h_p[Parker_Velocity_Control_B.i] =
          Parker_Velocity_Control_B.transpose_f[Parker_Velocity_Control_B.i + 16]
          * Parker_Velocity_Control_DW.BLUE_Tz +
          (Parker_Velocity_Control_B.transpose_f[Parker_Velocity_Control_B.i + 8]
           * Parker_Velocity_Control_B.rtb_Sum_idx_0 +
           Parker_Velocity_Control_B.transpose_f[Parker_Velocity_Control_B.i] *
           Parker_Velocity_Control_B.Subtract_o);
      }

      // End of Product: '<S46>/Product'

      // MATLAB Function: '<S46>/MATLAB Function6'
      Parker_Vel_MATLABFunction10(Parker_Velocity_Control_B.rtb_Product_h_p,
        &Parker_Velocity_Control_B.sf_MATLABFunction6);

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
      Parker_Velocity_Control_B.rtb_Sum_idx_0 =
        Parker_Velocity_Control_P.F_Blue_X_nominal *
        Parker_Velocity_Control_B.sf_MATLABFunction6.thrust_decay_factor / 2.0;
      Parker_Velocity_Control_B.rtb_Product_h_p[0] =
        Parker_Velocity_Control_B.rtb_Sum_idx_0;
      Parker_Velocity_Control_B.rtb_Product_h_p[1] =
        Parker_Velocity_Control_B.rtb_Sum_idx_0;
      Parker_Velocity_Control_B.VelocitytoPosition_idx_0 =
        Parker_Velocity_Control_P.F_Blue_Y_nominal *
        Parker_Velocity_Control_B.sf_MATLABFunction6.thrust_decay_factor / 2.0;
      Parker_Velocity_Control_B.rtb_Product_h_p[2] =
        Parker_Velocity_Control_B.VelocitytoPosition_idx_0;
      Parker_Velocity_Control_B.rtb_Product_h_p[3] =
        Parker_Velocity_Control_B.VelocitytoPosition_idx_0;
      Parker_Velocity_Control_B.rtb_Product_h_p[4] =
        Parker_Velocity_Control_B.rtb_Sum_idx_0;
      Parker_Velocity_Control_B.rtb_Product_h_p[5] =
        Parker_Velocity_Control_B.rtb_Sum_idx_0;
      Parker_Velocity_Control_B.rtb_Product_h_p[6] =
        Parker_Velocity_Control_B.VelocitytoPosition_idx_0;
      Parker_Velocity_Control_B.rtb_Product_h_p[7] =
        Parker_Velocity_Control_B.VelocitytoPosition_idx_0;
      memset(&Parker_Velocity_Control_B.Mat2[0], 0, sizeof(real_T) << 6U);

      // '<S52>:1:41' H    = Mat1*Mat2;
      for (Parker_Velocity_Control_B.uElOffset1 = 0;
           Parker_Velocity_Control_B.uElOffset1 < 8;
           Parker_Velocity_Control_B.uElOffset1++) {
        Parker_Velocity_Control_B.Mat2[Parker_Velocity_Control_B.uElOffset1 +
          (Parker_Velocity_Control_B.uElOffset1 << 3)] =
          Parker_Velocity_Control_B.rtb_Product_h_p[Parker_Velocity_Control_B.uElOffset1];
        Parker_Velocity_Control_B.b[3 * Parker_Velocity_Control_B.uElOffset1] =
          b[Parker_Velocity_Control_B.uElOffset1];
        Parker_Velocity_Control_B.b[3 * Parker_Velocity_Control_B.uElOffset1 + 1]
          = c[Parker_Velocity_Control_B.uElOffset1];
      }

      Parker_Velocity_Control_B.b[2] =
        Parker_Velocity_Control_P.thruster_dist2CG_BLUE[0] / 1000.0;
      Parker_Velocity_Control_B.b[5] =
        Parker_Velocity_Control_P.thruster_dist2CG_BLUE[1] / 1000.0;
      Parker_Velocity_Control_B.b[8] =
        Parker_Velocity_Control_P.thruster_dist2CG_BLUE[2] / 1000.0;
      Parker_Velocity_Control_B.b[11] =
        Parker_Velocity_Control_P.thruster_dist2CG_BLUE[3] / 1000.0;
      Parker_Velocity_Control_B.b[14] =
        Parker_Velocity_Control_P.thruster_dist2CG_BLUE[4] / 1000.0;
      Parker_Velocity_Control_B.b[17] =
        Parker_Velocity_Control_P.thruster_dist2CG_BLUE[5] / 1000.0;
      Parker_Velocity_Control_B.b[20] =
        Parker_Velocity_Control_P.thruster_dist2CG_BLUE[6] / 1000.0;
      Parker_Velocity_Control_B.b[23] =
        Parker_Velocity_Control_P.thruster_dist2CG_BLUE[7] / 1000.0;
      for (Parker_Velocity_Control_B.i = 0; Parker_Velocity_Control_B.i < 8;
           Parker_Velocity_Control_B.i++) {
        for (Parker_Velocity_Control_B.uElOffset1 = 0;
             Parker_Velocity_Control_B.uElOffset1 < 3;
             Parker_Velocity_Control_B.uElOffset1++) {
          Parker_Velocity_Control_B.ntIdx0 =
            Parker_Velocity_Control_B.uElOffset1 + 3 *
            Parker_Velocity_Control_B.i;
          Parker_Velocity_Control_B.Product2_a[Parker_Velocity_Control_B.ntIdx0]
            = 0.0;
          for (Parker_Velocity_Control_B.vcol = 0;
               Parker_Velocity_Control_B.vcol < 8;
               Parker_Velocity_Control_B.vcol++) {
            Parker_Velocity_Control_B.Product2_a[Parker_Velocity_Control_B.ntIdx0]
              += Parker_Velocity_Control_B.b[3 * Parker_Velocity_Control_B.vcol
              + Parker_Velocity_Control_B.uElOffset1] *
              Parker_Velocity_Control_B.Mat2[(Parker_Velocity_Control_B.i << 3)
              + Parker_Velocity_Control_B.vcol];
          }
        }
      }

      // End of MATLAB Function: '<S46>/MATLAB Function7'

      // PermuteDimensions: '<S55>/transpose'
      Parker_Velocity_Control_B.i = 0;
      Parker_Velocity_Control_B.uElOffset1 = 0;
      for (Parker_Velocity_Control_B.vcol = 0; Parker_Velocity_Control_B.vcol <
           3; Parker_Velocity_Control_B.vcol++) {
        Parker_Velocity_Control_B.ar = Parker_Velocity_Control_B.uElOffset1;
        for (Parker_Velocity_Control_B.ntIdx0 = 0;
             Parker_Velocity_Control_B.ntIdx0 < 8;
             Parker_Velocity_Control_B.ntIdx0++) {
          Parker_Velocity_Control_B.transpose_f[Parker_Velocity_Control_B.i] =
            Parker_Velocity_Control_B.Product2_a[Parker_Velocity_Control_B.ar];
          Parker_Velocity_Control_B.i++;
          Parker_Velocity_Control_B.ar += 3;
        }

        Parker_Velocity_Control_B.uElOffset1++;
      }

      // End of PermuteDimensions: '<S55>/transpose'

      // Product: '<S55>/Product'
      for (Parker_Velocity_Control_B.i = 0; Parker_Velocity_Control_B.i < 3;
           Parker_Velocity_Control_B.i++) {
        for (Parker_Velocity_Control_B.uElOffset1 = 0;
             Parker_Velocity_Control_B.uElOffset1 < 3;
             Parker_Velocity_Control_B.uElOffset1++) {
          Parker_Velocity_Control_B.ntIdx0 =
            Parker_Velocity_Control_B.uElOffset1 + 3 *
            Parker_Velocity_Control_B.i;
          Parker_Velocity_Control_B.rtb_H_hz_k[Parker_Velocity_Control_B.ntIdx0]
            = 0.0;
          for (Parker_Velocity_Control_B.vcol = 0;
               Parker_Velocity_Control_B.vcol < 8;
               Parker_Velocity_Control_B.vcol++) {
            Parker_Velocity_Control_B.rtb_H_hz_k[Parker_Velocity_Control_B.ntIdx0]
              += Parker_Velocity_Control_B.Product2_a[3 *
              Parker_Velocity_Control_B.vcol +
              Parker_Velocity_Control_B.uElOffset1] *
              Parker_Velocity_Control_B.transpose_f[(Parker_Velocity_Control_B.i
              << 3) + Parker_Velocity_Control_B.vcol];
          }
        }
      }

      // Product: '<S55>/Product1' incorporates:
      //   Product: '<S55>/Product'

      Parker_Velocity__rt_invd3x3_snf(Parker_Velocity_Control_B.rtb_H_hz_k,
        Parker_Velocity_Control_B.dv);
      for (Parker_Velocity_Control_B.i = 0; Parker_Velocity_Control_B.i < 8;
           Parker_Velocity_Control_B.i++) {
        // Product: '<S46>/Product4'
        Parker_Velocity_Control_B.rtb_transpose_f_c[Parker_Velocity_Control_B.i]
          = 0.0;
        for (Parker_Velocity_Control_B.uElOffset1 = 0;
             Parker_Velocity_Control_B.uElOffset1 < 3;
             Parker_Velocity_Control_B.uElOffset1++) {
          // Product: '<S55>/Product2' incorporates:
          //   Product: '<S46>/Product4'

          Parker_Velocity_Control_B.ntIdx0 = Parker_Velocity_Control_B.i +
            (Parker_Velocity_Control_B.uElOffset1 << 3);
          Parker_Velocity_Control_B.b[Parker_Velocity_Control_B.ntIdx0] = 0.0;
          Parker_Velocity_Control_B.b[Parker_Velocity_Control_B.ntIdx0] +=
            Parker_Velocity_Control_B.dv[3 *
            Parker_Velocity_Control_B.uElOffset1] *
            Parker_Velocity_Control_B.transpose_f[Parker_Velocity_Control_B.i];
          Parker_Velocity_Control_B.b[Parker_Velocity_Control_B.ntIdx0] +=
            Parker_Velocity_Control_B.dv[3 *
            Parker_Velocity_Control_B.uElOffset1 + 1] *
            Parker_Velocity_Control_B.transpose_f[Parker_Velocity_Control_B.i +
            8];
          Parker_Velocity_Control_B.b[Parker_Velocity_Control_B.ntIdx0] +=
            Parker_Velocity_Control_B.dv[3 *
            Parker_Velocity_Control_B.uElOffset1 + 2] *
            Parker_Velocity_Control_B.transpose_f[Parker_Velocity_Control_B.i +
            16];
          Parker_Velocity_Control_B.rtb_transpose_f_c[Parker_Velocity_Control_B.i]
            += Parker_Velocity_Control_B.b[Parker_Velocity_Control_B.ntIdx0] *
            Parker_Velocity_Control_B.x_ddot_b[Parker_Velocity_Control_B.uElOffset1];
        }

        // Saturate: '<S46>/Remove Negatives' incorporates:
        //   Product: '<S46>/Product4'
        //   Product: '<S55>/Product2'

        if (Parker_Velocity_Control_B.rtb_transpose_f_c[Parker_Velocity_Control_B.i]
            > Parker_Velocity_Control_P.RemoveNegatives_UpperSat_p) {
          Parker_Velocity_Control_B.rtb_Product_h_p[Parker_Velocity_Control_B.i]
            = Parker_Velocity_Control_P.RemoveNegatives_UpperSat_p;
        } else if
            (Parker_Velocity_Control_B.rtb_transpose_f_c[Parker_Velocity_Control_B.i]
             < Parker_Velocity_Control_P.RemoveNegatives_LowerSat_c) {
          Parker_Velocity_Control_B.rtb_Product_h_p[Parker_Velocity_Control_B.i]
            = Parker_Velocity_Control_P.RemoveNegatives_LowerSat_c;
        } else {
          Parker_Velocity_Control_B.rtb_Product_h_p[Parker_Velocity_Control_B.i]
            =
            Parker_Velocity_Control_B.rtb_transpose_f_c[Parker_Velocity_Control_B.i];
        }

        // End of Saturate: '<S46>/Remove Negatives'
      }

      // MATLAB Function: '<S46>/MATLAB Function2'
      Parker_Velo_MATLABFunction2(Parker_Velocity_Control_B.rtb_Product_h_p,
        &Parker_Velocity_Control_B.sf_MATLABFunction2_n);

      // Product: '<S46>/Product1'
      for (Parker_Velocity_Control_B.i = 0; Parker_Velocity_Control_B.i < 3;
           Parker_Velocity_Control_B.i++) {
        Parker_Velocity_Control_B.x_ddot_b[Parker_Velocity_Control_B.i] = 0.0;
        for (Parker_Velocity_Control_B.uElOffset1 = 0;
             Parker_Velocity_Control_B.uElOffset1 < 8;
             Parker_Velocity_Control_B.uElOffset1++) {
          Parker_Velocity_Control_B.x_ddot_b[Parker_Velocity_Control_B.i] +=
            Parker_Velocity_Control_B.sf_MATLABFunction1_b.H[3 *
            Parker_Velocity_Control_B.uElOffset1 + Parker_Velocity_Control_B.i] *
            Parker_Velocity_Control_B.sf_MATLABFunction2_n.ThrustPer_Final[Parker_Velocity_Control_B.uElOffset1];
        }
      }

      // End of Product: '<S46>/Product1'

      // DataStoreWrite: '<S46>/BLUE_Fx_Sat' incorporates:
      //   Product: '<S53>/Rotate F_b to F_I'

      Parker_Velocity_Control_DW.BLUE_Fx_Sat =
        Parker_Velocity_Control_B.sf_CreateRotationMatrix_i.C_Ib[0] *
        Parker_Velocity_Control_B.x_ddot_b[0] +
        Parker_Velocity_Control_B.sf_CreateRotationMatrix_i.C_Ib[2] *
        Parker_Velocity_Control_B.x_ddot_b[1];

      // DataStoreWrite: '<S46>/BLUE_Fy_Sat' incorporates:
      //   Product: '<S53>/Rotate F_b to F_I'

      Parker_Velocity_Control_DW.BLUE_Fy_Sat =
        Parker_Velocity_Control_B.sf_CreateRotationMatrix_i.C_Ib[1] *
        Parker_Velocity_Control_B.x_ddot_b[0] +
        Parker_Velocity_Control_B.sf_CreateRotationMatrix_i.C_Ib[3] *
        Parker_Velocity_Control_B.x_ddot_b[1];

      // DataStoreWrite: '<S46>/BLUE_Tz_Sat'
      Parker_Velocity_Control_DW.BLUE_Tz_Sat =
        Parker_Velocity_Control_B.x_ddot_b[2];

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

    if ((Parker_Velocity_Control_DW.WhoAmI == 1.0) ||
        (Parker_Velocity_Control_P.simMode == 1.0)) {
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
      for (Parker_Velocity_Control_B.i = 0; Parker_Velocity_Control_B.i < 8;
           Parker_Velocity_Control_B.i++) {
        Parker_Velocity_Control_B.rtb_Product_h_p[Parker_Velocity_Control_B.i] =
          Parker_Velocity_Control_P.F_thrusters_RED[Parker_Velocity_Control_B.i]
          / 2.0;
      }

      memset(&Parker_Velocity_Control_B.Mat2[0], 0, sizeof(real_T) << 6U);

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
      Parker_Velocity_Control_B.Sum6_d = cos
        (Parker_Velocity_Control_DW.RED_Rz_Shoulder + 1.5707963267948966);
      Parker_Velocity_Control_B.Sum6_l = Parker_Velocity_Control_P.model_param[7]
        * cos(Parker_Velocity_Control_P.model_param[6]) +
        Parker_Velocity_Control_P.model_param[8] *
        Parker_Velocity_Control_B.Sum6_d;

      // '<S64>:1:16' link1_com_y = b0*sin(phi) + a1*sin(pi/2 + theta_1);
      Parker_Velocity_Control_B.t30 = sin
        (Parker_Velocity_Control_DW.RED_Rz_Shoulder + 1.5707963267948966);
      Parker_Velocity_Control_B.Subtract_o =
        Parker_Velocity_Control_P.model_param[7] * sin
        (Parker_Velocity_Control_P.model_param[6]) +
        Parker_Velocity_Control_P.model_param[8] * Parker_Velocity_Control_B.t30;

      // '<S64>:1:18' link2_com_x = link1_com_x + b1*cos(pi/2 + theta_1) + a2*cos(pi/2 + theta_1 + theta_2); 
      Parker_Velocity_Control_B.t53 =
        (Parker_Velocity_Control_DW.RED_Rz_Shoulder + 1.5707963267948966) +
        Parker_Velocity_Control_DW.RED_Rz_Elbow;
      Parker_Velocity_Control_B.t31 = cos(Parker_Velocity_Control_B.t53);
      Parker_Velocity_Control_B.Sum6_d = (Parker_Velocity_Control_P.model_param
        [9] * Parker_Velocity_Control_B.Sum6_d +
        Parker_Velocity_Control_B.Sum6_l) +
        Parker_Velocity_Control_P.model_param[10] *
        Parker_Velocity_Control_B.t31;

      // '<S64>:1:19' link2_com_y = link1_com_y + b1*sin(pi/2 + theta_1) + a2*sin(pi/2 + theta_1 + theta_2); 
      Parker_Velocity_Control_B.rtb_Sum_h_idx_0 = sin
        (Parker_Velocity_Control_B.t53);
      Parker_Velocity_Control_B.t30 = (Parker_Velocity_Control_P.model_param[9] *
        Parker_Velocity_Control_B.t30 + Parker_Velocity_Control_B.Subtract_o) +
        Parker_Velocity_Control_P.model_param[10] *
        Parker_Velocity_Control_B.rtb_Sum_h_idx_0;

      // '<S64>:1:21' link3_com_x = link2_com_x + b2*cos(pi/2 + theta_1 + theta_2) + a3*cos(pi/2 + theta_1 + theta_2 + theta_3); 
      // '<S64>:1:22' link3_com_y = link2_com_y + b2*sin(pi/2 + theta_1 + theta_2) + a3*sin(pi/2 + theta_1 + theta_2 + theta_3); 
      // '<S64>:1:24' delta_com_due_to_arm_x = (m1*link1_com_x + m2*link2_com_x + m3*link3_com_x)/(m0 + m1 + m2 + m3)*1000; 
      Parker_Velocity_Control_B.t53 += Parker_Velocity_Control_DW.RED_Rz_Wrist;
      Parker_Velocity_Control_B.rtb_x_ddot_b_g =
        ((Parker_Velocity_Control_P.model_param[0] +
          Parker_Velocity_Control_P.model_param[14]) +
         Parker_Velocity_Control_P.model_param[15]) +
        Parker_Velocity_Control_P.model_param[16];
      Parker_Velocity_Control_B.VelocitytoPosition = cos
        (Parker_Velocity_Control_B.t53) * Parker_Velocity_Control_P.model_param
        [12];
      Parker_Velocity_Control_B.t31 =
        ((Parker_Velocity_Control_B.VelocitytoPosition +
          (Parker_Velocity_Control_P.model_param[11] *
           Parker_Velocity_Control_B.t31 + Parker_Velocity_Control_B.Sum6_d)) *
         Parker_Velocity_Control_P.model_param[16] +
         (Parker_Velocity_Control_P.model_param[14] *
          Parker_Velocity_Control_B.Sum6_l +
          Parker_Velocity_Control_P.model_param[15] *
          Parker_Velocity_Control_B.Sum6_d)) /
        Parker_Velocity_Control_B.rtb_x_ddot_b_g * 1000.0;

      // '<S64>:1:25' delta_com_due_to_arm_y = (m1*link1_com_y + m2*link2_com_y + m3*link3_com_y)/(m0 + m1 + m2 + m3)*1000; 
      Parker_Velocity_Control_B.t2 = sin(Parker_Velocity_Control_B.t53) *
        Parker_Velocity_Control_P.model_param[12];
      Parker_Velocity_Control_B.rtb_Sum_h_idx_0 = ((Parker_Velocity_Control_B.t2
        + (Parker_Velocity_Control_P.model_param[11] *
           Parker_Velocity_Control_B.rtb_Sum_h_idx_0 +
           Parker_Velocity_Control_B.t30)) *
        Parker_Velocity_Control_P.model_param[16] +
        (Parker_Velocity_Control_P.model_param[14] *
         Parker_Velocity_Control_B.Subtract_o +
         Parker_Velocity_Control_P.model_param[15] *
         Parker_Velocity_Control_B.t30)) /
        Parker_Velocity_Control_B.rtb_x_ddot_b_g * 1000.0;

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
      for (Parker_Velocity_Control_B.uElOffset1 = 0;
           Parker_Velocity_Control_B.uElOffset1 < 8;
           Parker_Velocity_Control_B.uElOffset1++) {
        // MATLAB Function: '<S58>/MATLAB Function3'
        Parker_Velocity_Control_B.Mat2[Parker_Velocity_Control_B.uElOffset1 +
          (Parker_Velocity_Control_B.uElOffset1 << 3)] =
          Parker_Velocity_Control_B.rtb_Product_h_p[Parker_Velocity_Control_B.uElOffset1];
        Parker_Velocity_Control_B.rtb_Product_h_p[Parker_Velocity_Control_B.uElOffset1]
          =
          Parker_Velocity_Control_P.F_thrusters_RED[Parker_Velocity_Control_B.uElOffset1]
          / 2.0;
      }

      // MATLAB Function: '<S58>/MATLAB Function'
      memset(&Parker_Velocity_Control_B.Mat2_m[0], 0, sizeof(real_T) << 6U);

      // If: '<S58>/If' incorporates:
      //   Constant: '<S58>/Constant'

      // '<S64>:1:62' H    = Mat1*Mat2;
      Parker_Velocity_Control_B.rtb_Compare_j =
        ((Parker_Velocity_Control_P.platformSelection == 4.0) ||
         (Parker_Velocity_Control_P.platformSelection == 5.0) ||
         (Parker_Velocity_Control_P.platformSelection == 10.0) ||
         (Parker_Velocity_Control_P.platformSelection == 11.0));

      // MATLAB Function: '<S58>/MATLAB Function'
      for (Parker_Velocity_Control_B.uElOffset1 = 0;
           Parker_Velocity_Control_B.uElOffset1 < 8;
           Parker_Velocity_Control_B.uElOffset1++) {
        Parker_Velocity_Control_B.Mat2_m[Parker_Velocity_Control_B.uElOffset1 +
          (Parker_Velocity_Control_B.uElOffset1 << 3)] =
          Parker_Velocity_Control_B.rtb_Product_h_p[Parker_Velocity_Control_B.uElOffset1];
        Parker_Velocity_Control_B.b[3 * Parker_Velocity_Control_B.uElOffset1] =
          b[Parker_Velocity_Control_B.uElOffset1];
        Parker_Velocity_Control_B.b[3 * Parker_Velocity_Control_B.uElOffset1 + 1]
          = c[Parker_Velocity_Control_B.uElOffset1];
      }

      Parker_Velocity_Control_B.b[2] =
        (Parker_Velocity_Control_P.thruster_dist2CG_RED[0] -
         Parker_Velocity_Control_B.rtb_Sum_h_idx_0) / 1000.0;
      Parker_Velocity_Control_B.b[5] =
        (Parker_Velocity_Control_P.thruster_dist2CG_RED[1] -
         Parker_Velocity_Control_B.rtb_Sum_h_idx_0) / 1000.0;
      Parker_Velocity_Control_B.b[8] =
        (Parker_Velocity_Control_P.thruster_dist2CG_RED[2] -
         Parker_Velocity_Control_B.t31) / 1000.0;
      Parker_Velocity_Control_B.b[11] =
        (Parker_Velocity_Control_P.thruster_dist2CG_RED[3] -
         Parker_Velocity_Control_B.t31) / 1000.0;
      Parker_Velocity_Control_B.b[14] =
        (Parker_Velocity_Control_P.thruster_dist2CG_RED[4] +
         Parker_Velocity_Control_B.rtb_Sum_h_idx_0) / 1000.0;
      Parker_Velocity_Control_B.b[17] =
        (Parker_Velocity_Control_P.thruster_dist2CG_RED[5] +
         Parker_Velocity_Control_B.rtb_Sum_h_idx_0) / 1000.0;
      Parker_Velocity_Control_B.b[20] =
        (Parker_Velocity_Control_P.thruster_dist2CG_RED[6] +
         Parker_Velocity_Control_B.t31) / 1000.0;
      Parker_Velocity_Control_B.b[23] =
        (Parker_Velocity_Control_P.thruster_dist2CG_RED[7] +
         Parker_Velocity_Control_B.t31) / 1000.0;
      for (Parker_Velocity_Control_B.i = 0; Parker_Velocity_Control_B.i < 8;
           Parker_Velocity_Control_B.i++) {
        for (Parker_Velocity_Control_B.uElOffset1 = 0;
             Parker_Velocity_Control_B.uElOffset1 < 3;
             Parker_Velocity_Control_B.uElOffset1++) {
          Parker_Velocity_Control_B.ntIdx0 =
            Parker_Velocity_Control_B.uElOffset1 + 3 *
            Parker_Velocity_Control_B.i;
          Parker_Velocity_Control_B.H_hz[Parker_Velocity_Control_B.ntIdx0] = 0.0;
          for (Parker_Velocity_Control_B.vcol = 0;
               Parker_Velocity_Control_B.vcol < 8;
               Parker_Velocity_Control_B.vcol++) {
            Parker_Velocity_Control_B.H_hz[Parker_Velocity_Control_B.ntIdx0] +=
              Parker_Velocity_Control_B.b[3 * Parker_Velocity_Control_B.vcol +
              Parker_Velocity_Control_B.uElOffset1] *
              Parker_Velocity_Control_B.Mat2_m[(Parker_Velocity_Control_B.i << 3)
              + Parker_Velocity_Control_B.vcol];
          }
        }

        // MATLAB Function: '<S58>/MATLAB Function3'
        Parker_Velocity_Control_B.Product2_a[3 * Parker_Velocity_Control_B.i] =
          b[Parker_Velocity_Control_B.i];
        Parker_Velocity_Control_B.Product2_a[3 * Parker_Velocity_Control_B.i + 1]
          = c[Parker_Velocity_Control_B.i];
        Parker_Velocity_Control_B.Product2_a[3 * Parker_Velocity_Control_B.i + 2]
          =
          Parker_Velocity_Control_P.thruster_dist2CG_RED[Parker_Velocity_Control_B.i]
          / 1000.0;
      }

      // MATLAB Function: '<S58>/MATLAB Function3'
      for (Parker_Velocity_Control_B.i = 0; Parker_Velocity_Control_B.i < 8;
           Parker_Velocity_Control_B.i++) {
        for (Parker_Velocity_Control_B.uElOffset1 = 0;
             Parker_Velocity_Control_B.uElOffset1 < 3;
             Parker_Velocity_Control_B.uElOffset1++) {
          Parker_Velocity_Control_B.ntIdx0 =
            Parker_Velocity_Control_B.uElOffset1 + 3 *
            Parker_Velocity_Control_B.i;
          Parker_Velocity_Control_B.transpose_f[Parker_Velocity_Control_B.ntIdx0]
            = 0.0;
          for (Parker_Velocity_Control_B.vcol = 0;
               Parker_Velocity_Control_B.vcol < 8;
               Parker_Velocity_Control_B.vcol++) {
            Parker_Velocity_Control_B.transpose_f[Parker_Velocity_Control_B.ntIdx0]
              += Parker_Velocity_Control_B.Product2_a[3 *
              Parker_Velocity_Control_B.vcol +
              Parker_Velocity_Control_B.uElOffset1] *
              Parker_Velocity_Control_B.Mat2[(Parker_Velocity_Control_B.i << 3)
              + Parker_Velocity_Control_B.vcol];
          }
        }
      }

      if (Parker_Velocity_Control_B.rtb_Compare_j) {
        // Outputs for IfAction SubSystem: '<S58>/If Action Subsystem' incorporates:
        //   ActionPort: '<S60>/Action Port'

        // Outputs for IfAction SubSystem: '<S58>/If Action Subsystem1' incorporates:
        //   ActionPort: '<S61>/Action Port'

        memcpy(&Parker_Velocity_Control_B.transpose_f[0],
               &Parker_Velocity_Control_B.H_hz[0], 24U * sizeof(real_T));

        // End of Outputs for SubSystem: '<S58>/If Action Subsystem1'
        // End of Outputs for SubSystem: '<S58>/If Action Subsystem'
      }

      // PermuteDimensions: '<S72>/transpose' incorporates:
      //   If: '<S58>/If'
      //   Inport: '<S60>/In1'
      //   Inport: '<S61>/In1'
      //   MATLAB Function: '<S58>/MATLAB Function'

      Parker_Velocity_Control_B.i = 0;
      Parker_Velocity_Control_B.uElOffset1 = 0;
      for (Parker_Velocity_Control_B.vcol = 0; Parker_Velocity_Control_B.vcol <
           3; Parker_Velocity_Control_B.vcol++) {
        Parker_Velocity_Control_B.ar = Parker_Velocity_Control_B.uElOffset1;
        for (Parker_Velocity_Control_B.ntIdx0 = 0;
             Parker_Velocity_Control_B.ntIdx0 < 8;
             Parker_Velocity_Control_B.ntIdx0++) {
          Parker_Velocity_Control_B.Product2_b[Parker_Velocity_Control_B.i] =
            Parker_Velocity_Control_B.transpose_f[Parker_Velocity_Control_B.ar];
          Parker_Velocity_Control_B.i++;
          Parker_Velocity_Control_B.ar += 3;
        }

        Parker_Velocity_Control_B.uElOffset1++;
      }

      // End of PermuteDimensions: '<S72>/transpose'

      // Product: '<S72>/Product'
      for (Parker_Velocity_Control_B.i = 0; Parker_Velocity_Control_B.i < 3;
           Parker_Velocity_Control_B.i++) {
        for (Parker_Velocity_Control_B.uElOffset1 = 0;
             Parker_Velocity_Control_B.uElOffset1 < 3;
             Parker_Velocity_Control_B.uElOffset1++) {
          Parker_Velocity_Control_B.ntIdx0 =
            Parker_Velocity_Control_B.uElOffset1 + 3 *
            Parker_Velocity_Control_B.i;
          Parker_Velocity_Control_B.rtb_H_hz_k[Parker_Velocity_Control_B.ntIdx0]
            = 0.0;
          for (Parker_Velocity_Control_B.vcol = 0;
               Parker_Velocity_Control_B.vcol < 8;
               Parker_Velocity_Control_B.vcol++) {
            Parker_Velocity_Control_B.rtb_H_hz_k[Parker_Velocity_Control_B.ntIdx0]
              += Parker_Velocity_Control_B.transpose_f[3 *
              Parker_Velocity_Control_B.vcol +
              Parker_Velocity_Control_B.uElOffset1] *
              Parker_Velocity_Control_B.Product2_b[(Parker_Velocity_Control_B.i <<
              3) + Parker_Velocity_Control_B.vcol];
          }
        }
      }

      // Product: '<S72>/Product1' incorporates:
      //   Product: '<S72>/Product'

      Parker_Velocity__rt_invd3x3_snf(Parker_Velocity_Control_B.rtb_H_hz_k,
        Parker_Velocity_Control_B.dv);

      // Product: '<S72>/Product2'
      for (Parker_Velocity_Control_B.i = 0; Parker_Velocity_Control_B.i < 3;
           Parker_Velocity_Control_B.i++) {
        for (Parker_Velocity_Control_B.uElOffset1 = 0;
             Parker_Velocity_Control_B.uElOffset1 < 8;
             Parker_Velocity_Control_B.uElOffset1++) {
          Parker_Velocity_Control_B.ntIdx0 =
            Parker_Velocity_Control_B.uElOffset1 + (Parker_Velocity_Control_B.i <<
            3);
          Parker_Velocity_Control_B.Product2_a[Parker_Velocity_Control_B.ntIdx0]
            = 0.0;
          Parker_Velocity_Control_B.Product2_a[Parker_Velocity_Control_B.ntIdx0]
            += Parker_Velocity_Control_B.dv[3 * Parker_Velocity_Control_B.i] *
            Parker_Velocity_Control_B.Product2_b[Parker_Velocity_Control_B.uElOffset1];
          Parker_Velocity_Control_B.Product2_a[Parker_Velocity_Control_B.ntIdx0]
            += Parker_Velocity_Control_B.dv[3 * Parker_Velocity_Control_B.i + 1]
            * Parker_Velocity_Control_B.Product2_b[Parker_Velocity_Control_B.uElOffset1
            + 8];
          Parker_Velocity_Control_B.Product2_a[Parker_Velocity_Control_B.ntIdx0]
            += Parker_Velocity_Control_B.dv[3 * Parker_Velocity_Control_B.i + 2]
            * Parker_Velocity_Control_B.Product2_b[Parker_Velocity_Control_B.uElOffset1
            + 16];
        }
      }

      // End of Product: '<S72>/Product2'

      // MATLAB Function: '<S59>/Create Rotation Matrix' incorporates:
      //   DataStoreRead: '<S31>/RED_Rz'

      Park_CreateRotationMatrix_h(Parker_Velocity_Control_DW.RED_Rz,
        &Parker_Velocity_Control_B.sf_CreateRotationMatrix_n);
      Parker_Velocity_Control_B.Subtract_o =
        Parker_Velocity_Control_B.sf_CreateRotationMatrix_n.C_bI[0] *
        Parker_Velocity_Control_DW.RED_Fx +
        Parker_Velocity_Control_B.sf_CreateRotationMatrix_n.C_bI[2] *
        Parker_Velocity_Control_DW.RED_Fy;

      // Product: '<S59>/Rotate F_I to F_b' incorporates:
      //   DataStoreRead: '<S31>/RED_Fx'
      //   DataStoreRead: '<S31>/RED_Fy'
      //   SignalConversion generated from: '<S59>/Rotate F_I to F_b'

      Parker_Velocity_Control_B.rtb_Sum_idx_0 =
        Parker_Velocity_Control_B.sf_CreateRotationMatrix_n.C_bI[1] *
        Parker_Velocity_Control_DW.RED_Fx +
        Parker_Velocity_Control_B.sf_CreateRotationMatrix_n.C_bI[3] *
        Parker_Velocity_Control_DW.RED_Fy;

      // SignalConversion generated from: '<S58>/Product' incorporates:
      //   DataStoreRead: '<S31>/RED_Tz'

      Parker_Velocity_Control_B.x_ddot_b[0] =
        Parker_Velocity_Control_B.Subtract_o;
      Parker_Velocity_Control_B.x_ddot_b[1] =
        Parker_Velocity_Control_B.rtb_Sum_idx_0;
      Parker_Velocity_Control_B.x_ddot_b[2] = Parker_Velocity_Control_DW.RED_Tz;

      // Product: '<S58>/Product' incorporates:
      //   DataStoreRead: '<S31>/RED_Tz'
      //   SignalConversion generated from: '<S58>/Product'

      for (Parker_Velocity_Control_B.i = 0; Parker_Velocity_Control_B.i < 8;
           Parker_Velocity_Control_B.i++) {
        Parker_Velocity_Control_B.rtb_Product_h_p[Parker_Velocity_Control_B.i] =
          Parker_Velocity_Control_B.Product2_a[Parker_Velocity_Control_B.i + 16]
          * Parker_Velocity_Control_DW.RED_Tz +
          (Parker_Velocity_Control_B.Product2_a[Parker_Velocity_Control_B.i + 8]
           * Parker_Velocity_Control_B.rtb_Sum_idx_0 +
           Parker_Velocity_Control_B.Product2_a[Parker_Velocity_Control_B.i] *
           Parker_Velocity_Control_B.Subtract_o);
      }

      // End of Product: '<S58>/Product'

      // MATLAB Function: '<S58>/MATLAB Function5'
      Parker_Vel_MATLABFunction10(Parker_Velocity_Control_B.rtb_Product_h_p,
        &Parker_Velocity_Control_B.sf_MATLABFunction5);

      // MATLAB Function: '<S71>/Create Rotation Matrix' incorporates:
      //   DataStoreRead: '<S58>/RED_Rz'

      Parker_CreateRotationMatrix(Parker_Velocity_Control_DW.RED_Rz,
        &Parker_Velocity_Control_B.sf_CreateRotationMatrix_e);

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
      memset(&Parker_Velocity_Control_B.Mat2[0], 0, sizeof(real_T) << 6U);
      for (Parker_Velocity_Control_B.uElOffset1 = 0;
           Parker_Velocity_Control_B.uElOffset1 < 8;
           Parker_Velocity_Control_B.uElOffset1++) {
        Parker_Velocity_Control_B.Mat2[Parker_Velocity_Control_B.uElOffset1 +
          (Parker_Velocity_Control_B.uElOffset1 << 3)] =
          Parker_Velocity_Control_P.F_thrusters_RED[Parker_Velocity_Control_B.uElOffset1];
      }

      // MATLAB Function: '<S58>/MATLAB Function4' incorporates:
      //   DataStoreRead: '<S58>/Data Store Read1'
      //   DataStoreRead: '<S58>/Data Store Read2'
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
      Parker_Velocity_Control_B.Sum6_l = cos
        (Parker_Velocity_Control_DW.RED_Rz_Shoulder + 1.5707963267948966) *
        Parker_Velocity_Control_P.model_param[8] +
        Parker_Velocity_Control_P.model_param[7] * cos
        (Parker_Velocity_Control_P.model_param[6]);

      // '<S68>:1:17' link1_com_y = b0*sin(phi) + a1*sin(pi/2 + theta_1);
      Parker_Velocity_Control_B.Subtract_o = sin
        (Parker_Velocity_Control_DW.RED_Rz_Shoulder + 1.5707963267948966) *
        Parker_Velocity_Control_P.model_param[8] +
        Parker_Velocity_Control_P.model_param[7] * sin
        (Parker_Velocity_Control_P.model_param[6]);

      // '<S68>:1:19' link2_com_x = link1_com_x + b1*cos(pi/2 + theta_1) + a2*cos(pi/2 + theta_1 + theta_2); 
      Parker_Velocity_Control_B.rtb_Sum_idx_1 = cos
        (Parker_Velocity_Control_DW.RED_Rz_Shoulder + 1.5707963267948966) *
        Parker_Velocity_Control_P.model_param[9];
      Parker_Velocity_Control_B.VelocitytoPosition_idx_1 = cos
        ((Parker_Velocity_Control_DW.RED_Rz_Shoulder + 1.5707963267948966) +
         Parker_Velocity_Control_DW.RED_Rz_Elbow) *
        Parker_Velocity_Control_P.model_param[10];
      Parker_Velocity_Control_B.Sum6_d =
        (Parker_Velocity_Control_B.rtb_Sum_idx_1 +
         Parker_Velocity_Control_B.Sum6_l) +
        Parker_Velocity_Control_B.VelocitytoPosition_idx_1;

      // '<S68>:1:20' link2_com_y = link1_com_y + b1*sin(pi/2 + theta_1) + a2*sin(pi/2 + theta_1 + theta_2); 
      Parker_Velocity_Control_B.t197 = sin
        (Parker_Velocity_Control_DW.RED_Rz_Shoulder + 1.5707963267948966) *
        Parker_Velocity_Control_P.model_param[9];
      Parker_Velocity_Control_B.rtb_Sum_idx_2 = sin
        ((Parker_Velocity_Control_DW.RED_Rz_Shoulder + 1.5707963267948966) +
         Parker_Velocity_Control_DW.RED_Rz_Elbow) *
        Parker_Velocity_Control_P.model_param[10];
      Parker_Velocity_Control_B.t30 = (Parker_Velocity_Control_B.t197 +
        Parker_Velocity_Control_B.Subtract_o) +
        Parker_Velocity_Control_B.rtb_Sum_idx_2;

      // '<S68>:1:22' link3_com_x = link2_com_x + b2*cos(pi/2 + theta_1 + theta_2) + a3*cos(pi/2 + theta_1 + theta_2 + theta_3); 
      // '<S68>:1:23' link3_com_y = link2_com_y + b2*sin(pi/2 + theta_1 + theta_2) + a3*sin(pi/2 + theta_1 + theta_2 + theta_3); 
      // '<S68>:1:25' delta_com_due_to_arm_x = (m1*link1_com_x + m2*link2_com_x + m3*link3_com_x)/(m0 + m1 + m2 + m3)*1000; 
      Parker_Velocity_Control_B.t53 = cos
        ((Parker_Velocity_Control_DW.RED_Rz_Shoulder + 1.5707963267948966) +
         Parker_Velocity_Control_DW.RED_Rz_Elbow) *
        Parker_Velocity_Control_P.model_param[11];
      Parker_Velocity_Control_B.t31 = (((Parker_Velocity_Control_B.t53 +
        Parker_Velocity_Control_B.Sum6_d) +
        Parker_Velocity_Control_B.VelocitytoPosition) *
        Parker_Velocity_Control_P.model_param[16] +
        (Parker_Velocity_Control_P.model_param[14] *
         Parker_Velocity_Control_B.Sum6_l +
         Parker_Velocity_Control_P.model_param[15] *
         Parker_Velocity_Control_B.Sum6_d)) /
        Parker_Velocity_Control_B.rtb_x_ddot_b_g * 1000.0;

      // '<S68>:1:26' delta_com_due_to_arm_y = (m1*link1_com_y + m2*link2_com_y + m3*link3_com_y)/(m0 + m1 + m2 + m3)*1000; 
      Parker_Velocity_Control_B.rtb_x_ddot_i_m = sin
        ((Parker_Velocity_Control_DW.RED_Rz_Shoulder + 1.5707963267948966) +
         Parker_Velocity_Control_DW.RED_Rz_Elbow) *
        Parker_Velocity_Control_P.model_param[11];
      Parker_Velocity_Control_B.rtb_Sum_h_idx_0 =
        (((Parker_Velocity_Control_B.rtb_x_ddot_i_m +
           Parker_Velocity_Control_B.t30) + Parker_Velocity_Control_B.t2) *
         Parker_Velocity_Control_P.model_param[16] +
         (Parker_Velocity_Control_P.model_param[14] *
          Parker_Velocity_Control_B.Subtract_o +
          Parker_Velocity_Control_P.model_param[15] *
          Parker_Velocity_Control_B.t30)) /
        Parker_Velocity_Control_B.rtb_x_ddot_b_g * 1000.0;

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
      Parker_Velocity_Control_B.rtb_Sum_idx_0 =
        Parker_Velocity_Control_P.F_red_X_nominal *
        Parker_Velocity_Control_B.sf_MATLABFunction5.thrust_decay_factor;
      Parker_Velocity_Control_B.rtb_Product_h_p[0] =
        Parker_Velocity_Control_B.rtb_Sum_idx_0;
      Parker_Velocity_Control_B.rtb_Product_h_p[1] =
        Parker_Velocity_Control_B.rtb_Sum_idx_0;
      Parker_Velocity_Control_B.VelocitytoPosition_idx_0 =
        Parker_Velocity_Control_P.F_red_Y_nominal *
        Parker_Velocity_Control_B.sf_MATLABFunction5.thrust_decay_factor;
      Parker_Velocity_Control_B.rtb_Product_h_p[2] =
        Parker_Velocity_Control_B.VelocitytoPosition_idx_0;
      Parker_Velocity_Control_B.rtb_Product_h_p[3] =
        Parker_Velocity_Control_B.VelocitytoPosition_idx_0;
      Parker_Velocity_Control_B.rtb_Product_h_p[4] =
        Parker_Velocity_Control_B.rtb_Sum_idx_0;
      Parker_Velocity_Control_B.rtb_Product_h_p[5] =
        Parker_Velocity_Control_B.rtb_Sum_idx_0;
      Parker_Velocity_Control_B.rtb_Product_h_p[6] =
        Parker_Velocity_Control_B.VelocitytoPosition_idx_0;
      Parker_Velocity_Control_B.rtb_Product_h_p[7] =
        Parker_Velocity_Control_B.VelocitytoPosition_idx_0;

      // '<S68>:1:63' Mat2 = diag((F_thrusters_RED));
      memset(&Parker_Velocity_Control_B.Mat2_m[0], 0, sizeof(real_T) << 6U);
      for (Parker_Velocity_Control_B.uElOffset1 = 0;
           Parker_Velocity_Control_B.uElOffset1 < 8;
           Parker_Velocity_Control_B.uElOffset1++) {
        Parker_Velocity_Control_B.Mat2_m[Parker_Velocity_Control_B.uElOffset1 +
          (Parker_Velocity_Control_B.uElOffset1 << 3)] =
          Parker_Velocity_Control_B.rtb_Product_h_p[Parker_Velocity_Control_B.uElOffset1];
      }

      // MATLAB Function: '<S58>/MATLAB Function6' incorporates:
      //   DataStoreRead: '<S58>/Data Store Read1'

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
      Parker_Velocity_Control_B.Sum6_l = cos
        (Parker_Velocity_Control_DW.RED_Rz_Shoulder + 1.5707963267948966) *
        Parker_Velocity_Control_P.model_param[8] +
        Parker_Velocity_Control_P.model_param[7] * cos
        (Parker_Velocity_Control_P.model_param[6]);

      // '<S70>:1:16' link1_com_y = b0*sin(phi) + a1*sin(pi/2 + theta_1);
      // '<S70>:1:18' link2_com_x = link1_com_x + b1*cos(pi/2 + theta_1) + a2*cos(pi/2 + theta_1 + theta_2); 
      Parker_Velocity_Control_B.Sum6_d =
        (Parker_Velocity_Control_B.rtb_Sum_idx_1 +
         Parker_Velocity_Control_B.Sum6_l) +
        Parker_Velocity_Control_B.VelocitytoPosition_idx_1;

      // '<S70>:1:19' link2_com_y = link1_com_y + b1*sin(pi/2 + theta_1) + a2*sin(pi/2 + theta_1 + theta_2); 
      Parker_Velocity_Control_B.t30 = (Parker_Velocity_Control_B.t197 +
        Parker_Velocity_Control_B.Subtract_o) +
        Parker_Velocity_Control_B.rtb_Sum_idx_2;

      // '<S70>:1:21' link3_com_x = link2_com_x + b2*cos(pi/2 + theta_1 + theta_2) + a3*cos(pi/2 + theta_1 + theta_2 + theta_3); 
      // '<S70>:1:22' link3_com_y = link2_com_y + b2*sin(pi/2 + theta_1 + theta_2) + a3*sin(pi/2 + theta_1 + theta_2 + theta_3); 
      // '<S70>:1:24' delta_com_due_to_arm_x = (m1*link1_com_x + m2*link2_com_x + m3*link3_com_x)/(m0 + m1 + m2 + m3)*1000; 
      Parker_Velocity_Control_B.Sum6_l = (((Parker_Velocity_Control_B.t53 +
        Parker_Velocity_Control_B.Sum6_d) +
        Parker_Velocity_Control_B.VelocitytoPosition) *
        Parker_Velocity_Control_P.model_param[16] +
        (Parker_Velocity_Control_P.model_param[14] *
         Parker_Velocity_Control_B.Sum6_l +
         Parker_Velocity_Control_P.model_param[15] *
         Parker_Velocity_Control_B.Sum6_d)) /
        Parker_Velocity_Control_B.rtb_x_ddot_b_g * 1000.0;

      // '<S70>:1:25' delta_com_due_to_arm_y = (m1*link1_com_y + m2*link2_com_y + m3*link3_com_y)/(m0 + m1 + m2 + m3)*1000; 
      Parker_Velocity_Control_B.Subtract_o =
        (((Parker_Velocity_Control_B.rtb_x_ddot_i_m +
           Parker_Velocity_Control_B.t30) + Parker_Velocity_Control_B.t2) *
         Parker_Velocity_Control_P.model_param[16] +
         (Parker_Velocity_Control_P.model_param[14] *
          Parker_Velocity_Control_B.Subtract_o +
          Parker_Velocity_Control_P.model_param[15] *
          Parker_Velocity_Control_B.t30)) /
        Parker_Velocity_Control_B.rtb_x_ddot_b_g * 1000.0;

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
      Parker_Velocity_Control_B.rtb_Sum_idx_0 /= 2.0;
      Parker_Velocity_Control_B.rtb_Product_h_p[0] =
        Parker_Velocity_Control_B.rtb_Sum_idx_0;
      Parker_Velocity_Control_B.rtb_Product_h_p[1] =
        Parker_Velocity_Control_B.rtb_Sum_idx_0;
      Parker_Velocity_Control_B.VelocitytoPosition_idx_0 /= 2.0;
      Parker_Velocity_Control_B.rtb_Product_h_p[2] =
        Parker_Velocity_Control_B.VelocitytoPosition_idx_0;
      Parker_Velocity_Control_B.rtb_Product_h_p[3] =
        Parker_Velocity_Control_B.VelocitytoPosition_idx_0;
      Parker_Velocity_Control_B.rtb_Product_h_p[4] =
        Parker_Velocity_Control_B.rtb_Sum_idx_0;
      Parker_Velocity_Control_B.rtb_Product_h_p[5] =
        Parker_Velocity_Control_B.rtb_Sum_idx_0;
      Parker_Velocity_Control_B.rtb_Product_h_p[6] =
        Parker_Velocity_Control_B.VelocitytoPosition_idx_0;
      Parker_Velocity_Control_B.rtb_Product_h_p[7] =
        Parker_Velocity_Control_B.VelocitytoPosition_idx_0;
      memset(&Parker_Velocity_Control_B.Mat2_c[0], 0, sizeof(real_T) << 6U);

      // '<S70>:1:64' H    = Mat1*Mat2;
      for (Parker_Velocity_Control_B.uElOffset1 = 0;
           Parker_Velocity_Control_B.uElOffset1 < 8;
           Parker_Velocity_Control_B.uElOffset1++) {
        Parker_Velocity_Control_B.Mat2_c[Parker_Velocity_Control_B.uElOffset1 +
          (Parker_Velocity_Control_B.uElOffset1 << 3)] =
          Parker_Velocity_Control_B.rtb_Product_h_p[Parker_Velocity_Control_B.uElOffset1];
        Parker_Velocity_Control_B.b[3 * Parker_Velocity_Control_B.uElOffset1] =
          b[Parker_Velocity_Control_B.uElOffset1];
        Parker_Velocity_Control_B.b[3 * Parker_Velocity_Control_B.uElOffset1 + 1]
          = c[Parker_Velocity_Control_B.uElOffset1];
      }

      Parker_Velocity_Control_B.b[2] =
        (Parker_Velocity_Control_P.thruster_dist2CG_RED[0] -
         Parker_Velocity_Control_B.Subtract_o) / 1000.0;
      Parker_Velocity_Control_B.b[5] =
        (Parker_Velocity_Control_P.thruster_dist2CG_RED[1] -
         Parker_Velocity_Control_B.Subtract_o) / 1000.0;
      Parker_Velocity_Control_B.b[8] =
        (Parker_Velocity_Control_P.thruster_dist2CG_RED[2] -
         Parker_Velocity_Control_B.Sum6_l) / 1000.0;
      Parker_Velocity_Control_B.b[11] =
        (Parker_Velocity_Control_P.thruster_dist2CG_RED[3] -
         Parker_Velocity_Control_B.Sum6_l) / 1000.0;
      Parker_Velocity_Control_B.b[14] =
        (Parker_Velocity_Control_P.thruster_dist2CG_RED[4] +
         Parker_Velocity_Control_B.Subtract_o) / 1000.0;
      Parker_Velocity_Control_B.b[17] =
        (Parker_Velocity_Control_P.thruster_dist2CG_RED[5] +
         Parker_Velocity_Control_B.Subtract_o) / 1000.0;
      Parker_Velocity_Control_B.b[20] =
        (Parker_Velocity_Control_P.thruster_dist2CG_RED[6] +
         Parker_Velocity_Control_B.Sum6_l) / 1000.0;
      Parker_Velocity_Control_B.b[23] =
        (Parker_Velocity_Control_P.thruster_dist2CG_RED[7] +
         Parker_Velocity_Control_B.Sum6_l) / 1000.0;
      for (Parker_Velocity_Control_B.i = 0; Parker_Velocity_Control_B.i < 8;
           Parker_Velocity_Control_B.i++) {
        for (Parker_Velocity_Control_B.uElOffset1 = 0;
             Parker_Velocity_Control_B.uElOffset1 < 3;
             Parker_Velocity_Control_B.uElOffset1++) {
          Parker_Velocity_Control_B.ntIdx0 =
            Parker_Velocity_Control_B.uElOffset1 + 3 *
            Parker_Velocity_Control_B.i;
          Parker_Velocity_Control_B.Product2_b[Parker_Velocity_Control_B.ntIdx0]
            = 0.0;
          for (Parker_Velocity_Control_B.vcol = 0;
               Parker_Velocity_Control_B.vcol < 8;
               Parker_Velocity_Control_B.vcol++) {
            Parker_Velocity_Control_B.Product2_b[Parker_Velocity_Control_B.ntIdx0]
              += Parker_Velocity_Control_B.b[3 * Parker_Velocity_Control_B.vcol
              + Parker_Velocity_Control_B.uElOffset1] *
              Parker_Velocity_Control_B.Mat2_c[(Parker_Velocity_Control_B.i << 3)
              + Parker_Velocity_Control_B.vcol];
          }
        }
      }

      // PermuteDimensions: '<S73>/transpose'
      Parker_Velocity_Control_B.i = 0;
      Parker_Velocity_Control_B.uElOffset1 = 0;
      for (Parker_Velocity_Control_B.vcol = 0; Parker_Velocity_Control_B.vcol <
           3; Parker_Velocity_Control_B.vcol++) {
        Parker_Velocity_Control_B.ar = Parker_Velocity_Control_B.uElOffset1;
        for (Parker_Velocity_Control_B.ntIdx0 = 0;
             Parker_Velocity_Control_B.ntIdx0 < 8;
             Parker_Velocity_Control_B.ntIdx0++) {
          Parker_Velocity_Control_B.Product2_a[Parker_Velocity_Control_B.i] =
            Parker_Velocity_Control_B.Product2_b[Parker_Velocity_Control_B.ar];
          Parker_Velocity_Control_B.i++;
          Parker_Velocity_Control_B.ar += 3;
        }

        Parker_Velocity_Control_B.uElOffset1++;
      }

      // End of PermuteDimensions: '<S73>/transpose'

      // Product: '<S73>/Product'
      for (Parker_Velocity_Control_B.i = 0; Parker_Velocity_Control_B.i < 3;
           Parker_Velocity_Control_B.i++) {
        for (Parker_Velocity_Control_B.uElOffset1 = 0;
             Parker_Velocity_Control_B.uElOffset1 < 3;
             Parker_Velocity_Control_B.uElOffset1++) {
          Parker_Velocity_Control_B.ntIdx0 =
            Parker_Velocity_Control_B.uElOffset1 + 3 *
            Parker_Velocity_Control_B.i;
          Parker_Velocity_Control_B.rtb_H_hz_k[Parker_Velocity_Control_B.ntIdx0]
            = 0.0;
          for (Parker_Velocity_Control_B.vcol = 0;
               Parker_Velocity_Control_B.vcol < 8;
               Parker_Velocity_Control_B.vcol++) {
            Parker_Velocity_Control_B.rtb_H_hz_k[Parker_Velocity_Control_B.ntIdx0]
              += Parker_Velocity_Control_B.Product2_b[3 *
              Parker_Velocity_Control_B.vcol +
              Parker_Velocity_Control_B.uElOffset1] *
              Parker_Velocity_Control_B.Product2_a[(Parker_Velocity_Control_B.i <<
              3) + Parker_Velocity_Control_B.vcol];
          }
        }
      }

      // Product: '<S73>/Product1' incorporates:
      //   Product: '<S73>/Product'

      Parker_Velocity__rt_invd3x3_snf(Parker_Velocity_Control_B.rtb_H_hz_k,
        Parker_Velocity_Control_B.dv);
      for (Parker_Velocity_Control_B.i = 0; Parker_Velocity_Control_B.i < 8;
           Parker_Velocity_Control_B.i++) {
        // Product: '<S58>/Product2'
        Parker_Velocity_Control_B.rtb_transpose_f_c[Parker_Velocity_Control_B.i]
          = 0.0;
        for (Parker_Velocity_Control_B.uElOffset1 = 0;
             Parker_Velocity_Control_B.uElOffset1 < 3;
             Parker_Velocity_Control_B.uElOffset1++) {
          // Product: '<S73>/Product2' incorporates:
          //   Product: '<S58>/Product2'

          Parker_Velocity_Control_B.ntIdx0 = Parker_Velocity_Control_B.i +
            (Parker_Velocity_Control_B.uElOffset1 << 3);
          Parker_Velocity_Control_B.transpose_f[Parker_Velocity_Control_B.ntIdx0]
            = 0.0;
          Parker_Velocity_Control_B.transpose_f[Parker_Velocity_Control_B.ntIdx0]
            += Parker_Velocity_Control_B.dv[3 *
            Parker_Velocity_Control_B.uElOffset1] *
            Parker_Velocity_Control_B.Product2_a[Parker_Velocity_Control_B.i];
          Parker_Velocity_Control_B.transpose_f[Parker_Velocity_Control_B.ntIdx0]
            += Parker_Velocity_Control_B.dv[3 *
            Parker_Velocity_Control_B.uElOffset1 + 1] *
            Parker_Velocity_Control_B.Product2_a[Parker_Velocity_Control_B.i + 8];
          Parker_Velocity_Control_B.transpose_f[Parker_Velocity_Control_B.ntIdx0]
            += Parker_Velocity_Control_B.dv[3 *
            Parker_Velocity_Control_B.uElOffset1 + 2] *
            Parker_Velocity_Control_B.Product2_a[Parker_Velocity_Control_B.i +
            16];
          Parker_Velocity_Control_B.rtb_transpose_f_c[Parker_Velocity_Control_B.i]
            +=
            Parker_Velocity_Control_B.transpose_f[Parker_Velocity_Control_B.ntIdx0]
            * Parker_Velocity_Control_B.x_ddot_b[Parker_Velocity_Control_B.uElOffset1];
        }

        // Saturate: '<S58>/Remove Negatives' incorporates:
        //   Product: '<S58>/Product2'
        //   Product: '<S73>/Product2'

        if (Parker_Velocity_Control_B.rtb_transpose_f_c[Parker_Velocity_Control_B.i]
            > Parker_Velocity_Control_P.RemoveNegatives_UpperSat_o) {
          Parker_Velocity_Control_B.rtb_Product_h_p[Parker_Velocity_Control_B.i]
            = Parker_Velocity_Control_P.RemoveNegatives_UpperSat_o;
        } else if
            (Parker_Velocity_Control_B.rtb_transpose_f_c[Parker_Velocity_Control_B.i]
             < Parker_Velocity_Control_P.RemoveNegatives_LowerSat_d) {
          Parker_Velocity_Control_B.rtb_Product_h_p[Parker_Velocity_Control_B.i]
            = Parker_Velocity_Control_P.RemoveNegatives_LowerSat_d;
        } else {
          Parker_Velocity_Control_B.rtb_Product_h_p[Parker_Velocity_Control_B.i]
            =
            Parker_Velocity_Control_B.rtb_transpose_f_c[Parker_Velocity_Control_B.i];
        }

        // End of Saturate: '<S58>/Remove Negatives'
      }

      // MATLAB Function: '<S58>/MATLAB Function2'
      Parker_Velo_MATLABFunction2(Parker_Velocity_Control_B.rtb_Product_h_p,
        &Parker_Velocity_Control_B.sf_MATLABFunction2_l);

      // If: '<S58>/If1' incorporates:
      //   Constant: '<S58>/Constant1'

      Parker_Velocity_Control_B.rtb_Compare_j =
        ((Parker_Velocity_Control_P.platformSelection == 4.0) ||
         (Parker_Velocity_Control_P.platformSelection == 5.0) ||
         (Parker_Velocity_Control_P.platformSelection == 10.0) ||
         (Parker_Velocity_Control_P.platformSelection == 11.0));

      // MATLAB Function: '<S58>/MATLAB Function4'
      for (Parker_Velocity_Control_B.i = 0; Parker_Velocity_Control_B.i < 8;
           Parker_Velocity_Control_B.i++) {
        Parker_Velocity_Control_B.b[3 * Parker_Velocity_Control_B.i] =
          b[Parker_Velocity_Control_B.i];
        Parker_Velocity_Control_B.b[3 * Parker_Velocity_Control_B.i + 1] =
          c[Parker_Velocity_Control_B.i];
      }

      Parker_Velocity_Control_B.b[2] =
        (Parker_Velocity_Control_P.thruster_dist2CG_RED[0] -
         Parker_Velocity_Control_B.rtb_Sum_h_idx_0) / 1000.0;
      Parker_Velocity_Control_B.b[5] =
        (Parker_Velocity_Control_P.thruster_dist2CG_RED[1] -
         Parker_Velocity_Control_B.rtb_Sum_h_idx_0) / 1000.0;
      Parker_Velocity_Control_B.b[8] =
        (Parker_Velocity_Control_P.thruster_dist2CG_RED[2] -
         Parker_Velocity_Control_B.t31) / 1000.0;
      Parker_Velocity_Control_B.b[11] =
        (Parker_Velocity_Control_P.thruster_dist2CG_RED[3] -
         Parker_Velocity_Control_B.t31) / 1000.0;
      Parker_Velocity_Control_B.b[14] =
        (Parker_Velocity_Control_P.thruster_dist2CG_RED[4] +
         Parker_Velocity_Control_B.rtb_Sum_h_idx_0) / 1000.0;
      Parker_Velocity_Control_B.b[17] =
        (Parker_Velocity_Control_P.thruster_dist2CG_RED[5] +
         Parker_Velocity_Control_B.rtb_Sum_h_idx_0) / 1000.0;
      Parker_Velocity_Control_B.b[20] =
        (Parker_Velocity_Control_P.thruster_dist2CG_RED[6] +
         Parker_Velocity_Control_B.t31) / 1000.0;
      Parker_Velocity_Control_B.b[23] =
        (Parker_Velocity_Control_P.thruster_dist2CG_RED[7] +
         Parker_Velocity_Control_B.t31) / 1000.0;
      for (Parker_Velocity_Control_B.i = 0; Parker_Velocity_Control_B.i < 8;
           Parker_Velocity_Control_B.i++) {
        for (Parker_Velocity_Control_B.uElOffset1 = 0;
             Parker_Velocity_Control_B.uElOffset1 < 3;
             Parker_Velocity_Control_B.uElOffset1++) {
          Parker_Velocity_Control_B.ntIdx0 =
            Parker_Velocity_Control_B.uElOffset1 + 3 *
            Parker_Velocity_Control_B.i;
          Parker_Velocity_Control_B.H_hz[Parker_Velocity_Control_B.ntIdx0] = 0.0;
          for (Parker_Velocity_Control_B.vcol = 0;
               Parker_Velocity_Control_B.vcol < 8;
               Parker_Velocity_Control_B.vcol++) {
            Parker_Velocity_Control_B.H_hz[Parker_Velocity_Control_B.ntIdx0] +=
              Parker_Velocity_Control_B.b[3 * Parker_Velocity_Control_B.vcol +
              Parker_Velocity_Control_B.uElOffset1] *
              Parker_Velocity_Control_B.Mat2_m[(Parker_Velocity_Control_B.i << 3)
              + Parker_Velocity_Control_B.vcol];
          }
        }

        // MATLAB Function: '<S58>/MATLAB Function1'
        Parker_Velocity_Control_B.Product2_a[3 * Parker_Velocity_Control_B.i] =
          b[Parker_Velocity_Control_B.i];
        Parker_Velocity_Control_B.Product2_a[3 * Parker_Velocity_Control_B.i + 1]
          = c[Parker_Velocity_Control_B.i];
        Parker_Velocity_Control_B.Product2_a[3 * Parker_Velocity_Control_B.i + 2]
          =
          Parker_Velocity_Control_P.thruster_dist2CG_RED[Parker_Velocity_Control_B.i]
          / 1000.0;
      }

      // MATLAB Function: '<S58>/MATLAB Function1'
      for (Parker_Velocity_Control_B.i = 0; Parker_Velocity_Control_B.i < 8;
           Parker_Velocity_Control_B.i++) {
        for (Parker_Velocity_Control_B.uElOffset1 = 0;
             Parker_Velocity_Control_B.uElOffset1 < 3;
             Parker_Velocity_Control_B.uElOffset1++) {
          Parker_Velocity_Control_B.ntIdx0 =
            Parker_Velocity_Control_B.uElOffset1 + 3 *
            Parker_Velocity_Control_B.i;
          Parker_Velocity_Control_B.b[Parker_Velocity_Control_B.ntIdx0] = 0.0;
          for (Parker_Velocity_Control_B.vcol = 0;
               Parker_Velocity_Control_B.vcol < 8;
               Parker_Velocity_Control_B.vcol++) {
            Parker_Velocity_Control_B.b[Parker_Velocity_Control_B.ntIdx0] +=
              Parker_Velocity_Control_B.Product2_a[3 *
              Parker_Velocity_Control_B.vcol +
              Parker_Velocity_Control_B.uElOffset1] *
              Parker_Velocity_Control_B.Mat2[(Parker_Velocity_Control_B.i << 3)
              + Parker_Velocity_Control_B.vcol];
          }
        }
      }

      if (Parker_Velocity_Control_B.rtb_Compare_j) {
        memcpy(&Parker_Velocity_Control_B.b[0], &Parker_Velocity_Control_B.H_hz
               [0], 24U * sizeof(real_T));
      }

      // Product: '<S58>/Product1' incorporates:
      //   If: '<S58>/If1'
      //   MATLAB Function: '<S58>/MATLAB Function4'

      for (Parker_Velocity_Control_B.i = 0; Parker_Velocity_Control_B.i < 3;
           Parker_Velocity_Control_B.i++) {
        Parker_Velocity_Control_B.x_ddot_b[Parker_Velocity_Control_B.i] = 0.0;
        for (Parker_Velocity_Control_B.uElOffset1 = 0;
             Parker_Velocity_Control_B.uElOffset1 < 8;
             Parker_Velocity_Control_B.uElOffset1++) {
          Parker_Velocity_Control_B.x_ddot_b[Parker_Velocity_Control_B.i] +=
            Parker_Velocity_Control_B.b[3 * Parker_Velocity_Control_B.uElOffset1
            + Parker_Velocity_Control_B.i] *
            Parker_Velocity_Control_B.sf_MATLABFunction2_l.ThrustPer_Final[Parker_Velocity_Control_B.uElOffset1];
        }
      }

      // End of Product: '<S58>/Product1'

      // DataStoreWrite: '<S58>/RED_Fx_Sat' incorporates:
      //   Product: '<S71>/Rotate F_b to F_I'

      Parker_Velocity_Control_DW.RED_Fx_Sat =
        Parker_Velocity_Control_B.sf_CreateRotationMatrix_e.C_Ib[0] *
        Parker_Velocity_Control_B.x_ddot_b[0] +
        Parker_Velocity_Control_B.sf_CreateRotationMatrix_e.C_Ib[2] *
        Parker_Velocity_Control_B.x_ddot_b[1];

      // DataStoreWrite: '<S58>/RED_Fy_Sat' incorporates:
      //   Product: '<S71>/Rotate F_b to F_I'

      Parker_Velocity_Control_DW.RED_Fy_Sat =
        Parker_Velocity_Control_B.sf_CreateRotationMatrix_e.C_Ib[1] *
        Parker_Velocity_Control_B.x_ddot_b[0] +
        Parker_Velocity_Control_B.sf_CreateRotationMatrix_e.C_Ib[3] *
        Parker_Velocity_Control_B.x_ddot_b[1];

      // DataStoreWrite: '<S58>/RED_Tz_Sat'
      Parker_Velocity_Control_DW.RED_Tz_Sat =
        Parker_Velocity_Control_B.x_ddot_b[2];

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

    if (Parker_Velocity_Control_M->Timing.RateInteraction.TID1_2) {
      Parker_Velocity_Control_DW.RateTransition_Buffer =
        (Parker_Velocity_Control_B.sf_MATLABFunction2_l.ThrustPer_Final[0] +
         Parker_Velocity_Control_B.sf_MATLABFunction2.ThrustPer_Final[7]) +
        Parker_Velocity_Control_B.sf_MATLABFunction2_n.ThrustPer_Final[7];
      Parker_Velocity_Control_DW.RateTransition1_Buffer =
        (Parker_Velocity_Control_B.sf_MATLABFunction2_l.ThrustPer_Final[1] +
         Parker_Velocity_Control_B.sf_MATLABFunction2.ThrustPer_Final[2]) +
        Parker_Velocity_Control_B.sf_MATLABFunction2_n.ThrustPer_Final[2];

      // RateTransition: '<S4>/Rate Transition2' incorporates:
      //   Sum: '<S4>/Sum'
      //   Sum: '<S4>/Sum1'

      Parker_Velocity_Control_DW.RateTransition2_Buffer =
        Parker_Velocity_Control_B.sf_MATLABFunction2_l.ThrustPer_Final[2];
      Parker_Velocity_Control_DW.RateTransition3_Buffer =
        (Parker_Velocity_Control_B.sf_MATLABFunction2_l.ThrustPer_Final[3] +
         Parker_Velocity_Control_B.sf_MATLABFunction2.ThrustPer_Final[4]) +
        Parker_Velocity_Control_B.sf_MATLABFunction2_n.ThrustPer_Final[4];
      Parker_Velocity_Control_DW.RateTransition4_Buffer =
        (Parker_Velocity_Control_B.sf_MATLABFunction2_l.ThrustPer_Final[4] +
         Parker_Velocity_Control_B.sf_MATLABFunction2.ThrustPer_Final[3]) +
        Parker_Velocity_Control_B.sf_MATLABFunction2_n.ThrustPer_Final[3];
      Parker_Velocity_Control_DW.RateTransition5_Buffer =
        (Parker_Velocity_Control_B.sf_MATLABFunction2_l.ThrustPer_Final[5] +
         Parker_Velocity_Control_B.sf_MATLABFunction2.ThrustPer_Final[6]) +
        Parker_Velocity_Control_B.sf_MATLABFunction2_n.ThrustPer_Final[6];
      Parker_Velocity_Control_DW.RateTransition6_Buffer =
        (Parker_Velocity_Control_B.sf_MATLABFunction2_l.ThrustPer_Final[6] +
         Parker_Velocity_Control_B.sf_MATLABFunction2.ThrustPer_Final[5]) +
        Parker_Velocity_Control_B.sf_MATLABFunction2_n.ThrustPer_Final[5];
      Parker_Velocity_Control_DW.RateTransition7_Buffer =
        (Parker_Velocity_Control_B.sf_MATLABFunction2_l.ThrustPer_Final[7] +
         Parker_Velocity_Control_B.sf_MATLABFunction2.ThrustPer_Final[0]) +
        Parker_Velocity_Control_B.sf_MATLABFunction2_n.ThrustPer_Final[0];
      Parker_Velocity_Control_DW.RateTransition8_Buffer =
        Parker_Velocity_Control_B.sf_MATLABFunction2.ThrustPer_Final[1] +
        Parker_Velocity_Control_B.sf_MATLABFunction2_n.ThrustPer_Final[1];
    }

    // End of RateTransition: '<S4>/Rate Transition'

    // If: '<S6>/If performing an experiment, grab the PhaseSpace data. Otherwise, use a clock to set time in SIM.' incorporates:
    //   Constant: '<S96>/Constant'

    Parker_Velocity_Control_B.rtAction = -1;
    if (Parker_Velocity_Control_P.simMode == 0.0) {
      Parker_Velocity_Control_B.rtAction = 0;

      // Outputs for IfAction SubSystem: '<S6>/Use Hardware to Obtain States' incorporates:
      //   ActionPort: '<S97>/Action Port'

      // S-Function (sdspFromNetwork): '<S97>/UDP Receive'
      sErr = GetErrorBuffer(&Parker_Velocity_Control_DW.UDPReceive_NetworkLib[0U]);
      Parker_Velocity_Control_B.i = 16;
      LibOutputs_Network(&Parker_Velocity_Control_DW.UDPReceive_NetworkLib[0U],
                         &Parker_Velocity_Control_B.UDPReceive_o1[0U],
                         &Parker_Velocity_Control_B.i);
      if (*sErr != 0) {
        rtmSetErrorStatus(Parker_Velocity_Control_M, sErr);
        rtmSetStopRequested(Parker_Velocity_Control_M, 1);
      }

      // End of S-Function (sdspFromNetwork): '<S97>/UDP Receive'

      // DataStoreWrite: '<S97>/Data Store Write3'
      Parker_Velocity_Control_DW.ARM_Elbow_Px =
        Parker_Velocity_Control_B.UDPReceive_o1[7];

      // DataStoreWrite: '<S97>/Data Store Write4'
      Parker_Velocity_Control_DW.ARM_Elbow_Py =
        Parker_Velocity_Control_B.UDPReceive_o1[8];

      // DataStoreWrite: '<S97>/Data Store Write5'
      Parker_Velocity_Control_DW.ARM_Wrist_Px =
        Parker_Velocity_Control_B.UDPReceive_o1[9];

      // DataStoreWrite: '<S97>/Data Store Write6'
      Parker_Velocity_Control_DW.ARM_Wrist_Py =
        Parker_Velocity_Control_B.UDPReceive_o1[10];

      // DataStoreWrite: '<S97>/Data Store Write7'
      Parker_Velocity_Control_DW.ARM_EndEff_Px =
        Parker_Velocity_Control_B.UDPReceive_o1[11];

      // DataStoreWrite: '<S97>/Data Store Write8'
      Parker_Velocity_Control_DW.ARM_EndEff_Py =
        Parker_Velocity_Control_B.UDPReceive_o1[12];

      // DataStoreWrite: '<S97>/RED_Px1'
      Parker_Velocity_Control_DW.RED_Px =
        Parker_Velocity_Control_B.UDPReceive_o1[1];

      // DataStoreWrite: '<S97>/RED_Px10'
      Parker_Velocity_Control_DW.Univ_Time =
        Parker_Velocity_Control_B.UDPReceive_o1[0];

      // Delay: '<S107>/Delay1'
      if (Parker_Velocity_Control_DW.icLoad_c != 0) {
        Parker_Velocity_Control_DW.Delay1_DSTATE_hi =
          Parker_Velocity_Control_B.UDPReceive_o1[4];
      }

      // Sum: '<S107>/Sum6' incorporates:
      //   Delay: '<S107>/Delay1'

      Parker_Velocity_Control_B.Subtract_o =
        Parker_Velocity_Control_B.UDPReceive_o1[4] -
        Parker_Velocity_Control_DW.Delay1_DSTATE_hi;

      // If: '<S107>/if we went through a "step"' incorporates:
      //   Inport: '<S122>/In1'

      if (Parker_Velocity_Control_B.Subtract_o != 0.0) {
        // Outputs for IfAction SubSystem: '<S107>/Hold this value' incorporates:
        //   ActionPort: '<S122>/Action Port'

        Parker_Velocity_Control_B.In1_gw = Parker_Velocity_Control_B.Subtract_o;

        // End of Outputs for SubSystem: '<S107>/Hold this value'
      }

      // End of If: '<S107>/if we went through a "step"'

      // Gain: '<S107>/divide by delta T'
      Parker_Velocity_Control_B.dividebydeltaT = 1.0 /
        Parker_Velocity_Control_P.baseRate * Parker_Velocity_Control_B.In1_gw;

      // Delay: '<S99>/Delay1'
      if (Parker_Velocity_Control_DW.icLoad_h0 != 0) {
        Parker_Velocity_Control_DW.Delay1_DSTATE_df =
          Parker_Velocity_Control_B.dividebydeltaT;
      }

      // Sum: '<S99>/Sum6' incorporates:
      //   Delay: '<S99>/Delay1'

      Parker_Velocity_Control_B.Subtract_o =
        Parker_Velocity_Control_B.dividebydeltaT -
        Parker_Velocity_Control_DW.Delay1_DSTATE_df;

      // If: '<S99>/if we went through a "step"' incorporates:
      //   Inport: '<S114>/In1'

      if (Parker_Velocity_Control_B.Subtract_o != 0.0) {
        // Outputs for IfAction SubSystem: '<S99>/Hold this value' incorporates:
        //   ActionPort: '<S114>/Action Port'

        Parker_Velocity_Control_B.In1_cw = Parker_Velocity_Control_B.Subtract_o;

        // End of Outputs for SubSystem: '<S99>/Hold this value'
      }

      // End of If: '<S99>/if we went through a "step"'

      // Gain: '<S99>/divide by delta T' incorporates:
      //   DataStoreWrite: '<S97>/RED_Px11'

      Parker_Velocity_Control_DW.BLACK_Ax = 1.0 /
        Parker_Velocity_Control_P.baseRate * Parker_Velocity_Control_B.In1_cw;

      // Delay: '<S108>/Delay1'
      if (Parker_Velocity_Control_DW.icLoad_kt != 0) {
        Parker_Velocity_Control_DW.Delay1_DSTATE_ft =
          Parker_Velocity_Control_B.UDPReceive_o1[5];
      }

      // Sum: '<S108>/Sum6' incorporates:
      //   Delay: '<S108>/Delay1'

      Parker_Velocity_Control_B.Subtract_o =
        Parker_Velocity_Control_B.UDPReceive_o1[5] -
        Parker_Velocity_Control_DW.Delay1_DSTATE_ft;

      // If: '<S108>/if we went through a "step"' incorporates:
      //   Inport: '<S123>/In1'

      if (Parker_Velocity_Control_B.Subtract_o != 0.0) {
        // Outputs for IfAction SubSystem: '<S108>/Hold this value' incorporates:
        //   ActionPort: '<S123>/Action Port'

        Parker_Velocity_Control_B.In1_k5 = Parker_Velocity_Control_B.Subtract_o;

        // End of Outputs for SubSystem: '<S108>/Hold this value'
      }

      // End of If: '<S108>/if we went through a "step"'

      // Gain: '<S108>/divide by delta T'
      Parker_Velocity_Control_B.dividebydeltaT_k = 1.0 /
        Parker_Velocity_Control_P.baseRate * Parker_Velocity_Control_B.In1_k5;

      // Delay: '<S100>/Delay1'
      if (Parker_Velocity_Control_DW.icLoad_ktm != 0) {
        Parker_Velocity_Control_DW.Delay1_DSTATE_hz =
          Parker_Velocity_Control_B.dividebydeltaT_k;
      }

      // Sum: '<S100>/Sum6' incorporates:
      //   Delay: '<S100>/Delay1'

      Parker_Velocity_Control_B.Subtract_o =
        Parker_Velocity_Control_B.dividebydeltaT_k -
        Parker_Velocity_Control_DW.Delay1_DSTATE_hz;

      // If: '<S100>/if we went through a "step"' incorporates:
      //   Inport: '<S115>/In1'

      if (Parker_Velocity_Control_B.Subtract_o != 0.0) {
        // Outputs for IfAction SubSystem: '<S100>/Hold this value' incorporates:
        //   ActionPort: '<S115>/Action Port'

        Parker_Velocity_Control_B.In1_gp = Parker_Velocity_Control_B.Subtract_o;

        // End of Outputs for SubSystem: '<S100>/Hold this value'
      }

      // End of If: '<S100>/if we went through a "step"'

      // Gain: '<S100>/divide by delta T' incorporates:
      //   DataStoreWrite: '<S97>/RED_Px12'

      Parker_Velocity_Control_DW.BLACK_Ay = 1.0 /
        Parker_Velocity_Control_P.baseRate * Parker_Velocity_Control_B.In1_gp;

      // S-Function (sdspunwrap2): '<S97>/Unwrap1'
      if (Parker_Velocity_Control_DW.Unwrap1_FirstStep) {
        Parker_Velocity_Control_B.i = 0;
        while (Parker_Velocity_Control_B.i < 1) {
          Parker_Velocity_Control_DW.Unwrap1_Prev =
            Parker_Velocity_Control_B.UDPReceive_o1[6];
          Parker_Velocity_Control_B.i++;
        }

        Parker_Velocity_Control_DW.Unwrap1_FirstStep = false;
      }

      Parker_Velocity_Control_B.i = 0;
      while (Parker_Velocity_Control_B.i < 1) {
        Parker_Velocity_Control_B.Subtract_o =
          Parker_Velocity_Control_DW.Unwrap1_Cumsum;
        Parker_Velocity_Control_B.Sum6_l =
          Parker_Velocity_Control_B.UDPReceive_o1[6] -
          Parker_Velocity_Control_DW.Unwrap1_Prev;
        Parker_Velocity_Control_B.Sum6_d = Parker_Velocity_Control_B.Sum6_l -
          floor((Parker_Velocity_Control_B.Sum6_l + 3.1415926535897931) /
                6.2831853071795862) * 6.2831853071795862;
        if ((Parker_Velocity_Control_B.Sum6_d == -3.1415926535897931) &&
            (Parker_Velocity_Control_B.Sum6_l > 0.0)) {
          Parker_Velocity_Control_B.Sum6_d = 3.1415926535897931;
        }

        Parker_Velocity_Control_B.Sum6_l = Parker_Velocity_Control_B.Sum6_d -
          Parker_Velocity_Control_B.Sum6_l;
        if (fabs(Parker_Velocity_Control_B.Sum6_l) > 3.1415926535897931) {
          Parker_Velocity_Control_B.Subtract_o =
            Parker_Velocity_Control_DW.Unwrap1_Cumsum +
            Parker_Velocity_Control_B.Sum6_l;
        }

        Parker_Velocity_Control_DW.Unwrap1_Prev =
          Parker_Velocity_Control_B.UDPReceive_o1[6];
        Parker_Velocity_Control_B.Unwrap1 =
          Parker_Velocity_Control_B.UDPReceive_o1[6] +
          Parker_Velocity_Control_B.Subtract_o;
        Parker_Velocity_Control_DW.Unwrap1_Cumsum =
          Parker_Velocity_Control_B.Subtract_o;
        Parker_Velocity_Control_B.i++;
      }

      // End of S-Function (sdspunwrap2): '<S97>/Unwrap1'

      // Delay: '<S109>/Delay1'
      if (Parker_Velocity_Control_DW.icLoad_gn != 0) {
        Parker_Velocity_Control_DW.Delay1_DSTATE_mt =
          Parker_Velocity_Control_B.Unwrap1;
      }

      // Sum: '<S109>/Sum6' incorporates:
      //   Delay: '<S109>/Delay1'

      Parker_Velocity_Control_B.Subtract_o = Parker_Velocity_Control_B.Unwrap1 -
        Parker_Velocity_Control_DW.Delay1_DSTATE_mt;

      // If: '<S109>/if we went through a "step"' incorporates:
      //   Inport: '<S124>/In1'

      if (Parker_Velocity_Control_B.Subtract_o != 0.0) {
        // Outputs for IfAction SubSystem: '<S109>/Hold this value' incorporates:
        //   ActionPort: '<S124>/Action Port'

        Parker_Velocity_Control_B.In1_ex = Parker_Velocity_Control_B.Subtract_o;

        // End of Outputs for SubSystem: '<S109>/Hold this value'
      }

      // End of If: '<S109>/if we went through a "step"'

      // Gain: '<S109>/divide by delta T'
      Parker_Velocity_Control_B.dividebydeltaT_d = 1.0 /
        Parker_Velocity_Control_P.baseRate * Parker_Velocity_Control_B.In1_ex;

      // Delay: '<S101>/Delay1'
      if (Parker_Velocity_Control_DW.icLoad_pz != 0) {
        Parker_Velocity_Control_DW.Delay1_DSTATE_lf =
          Parker_Velocity_Control_B.dividebydeltaT_d;
      }

      // Sum: '<S101>/Sum6' incorporates:
      //   Delay: '<S101>/Delay1'

      Parker_Velocity_Control_B.Subtract_o =
        Parker_Velocity_Control_B.dividebydeltaT_d -
        Parker_Velocity_Control_DW.Delay1_DSTATE_lf;

      // If: '<S101>/if we went through a "step"' incorporates:
      //   Inport: '<S116>/In1'

      if (Parker_Velocity_Control_B.Subtract_o != 0.0) {
        // Outputs for IfAction SubSystem: '<S101>/Hold this value' incorporates:
        //   ActionPort: '<S116>/Action Port'

        Parker_Velocity_Control_B.In1_n = Parker_Velocity_Control_B.Subtract_o;

        // End of Outputs for SubSystem: '<S101>/Hold this value'
      }

      // End of If: '<S101>/if we went through a "step"'

      // Gain: '<S101>/divide by delta T' incorporates:
      //   DataStoreWrite: '<S97>/RED_Px13'

      Parker_Velocity_Control_DW.BLACK_RzDD = 1.0 /
        Parker_Velocity_Control_P.baseRate * Parker_Velocity_Control_B.In1_n;

      // Delay: '<S102>/Delay1'
      if (Parker_Velocity_Control_DW.icLoad_fy != 0) {
        Parker_Velocity_Control_DW.Delay1_DSTATE_cs =
          Parker_Velocity_Control_B.UDPReceive_o1[13];
      }

      // Sum: '<S102>/Sum6' incorporates:
      //   Delay: '<S102>/Delay1'

      Parker_Velocity_Control_B.Subtract_o =
        Parker_Velocity_Control_B.UDPReceive_o1[13] -
        Parker_Velocity_Control_DW.Delay1_DSTATE_cs;

      // If: '<S102>/if we went through a "step"' incorporates:
      //   Inport: '<S117>/In1'

      if (Parker_Velocity_Control_B.Subtract_o != 0.0) {
        // Outputs for IfAction SubSystem: '<S102>/Hold this value' incorporates:
        //   ActionPort: '<S117>/Action Port'

        Parker_Velocity_Control_B.In1_i0 = Parker_Velocity_Control_B.Subtract_o;

        // End of Outputs for SubSystem: '<S102>/Hold this value'
      }

      // End of If: '<S102>/if we went through a "step"'

      // Gain: '<S102>/divide by delta T' incorporates:
      //   DataStoreWrite: '<S97>/RED_Px17'

      Parker_Velocity_Control_DW.BLUE_Vx = 1.0 /
        Parker_Velocity_Control_P.baseRate * Parker_Velocity_Control_B.In1_i0;

      // DataStoreWrite: '<S97>/RED_Px18'
      Parker_Velocity_Control_DW.BLUE_Px =
        Parker_Velocity_Control_B.UDPReceive_o1[13];

      // DataStoreWrite: '<S97>/RED_Px19'
      Parker_Velocity_Control_DW.BLUE_Py =
        Parker_Velocity_Control_B.UDPReceive_o1[14];

      // Delay: '<S98>/Delay1'
      if (Parker_Velocity_Control_DW.icLoad_ea != 0) {
        Parker_Velocity_Control_DW.Delay1_DSTATE_kv =
          Parker_Velocity_Control_B.UDPReceive_o1[1];
      }

      // Sum: '<S98>/Sum6' incorporates:
      //   Delay: '<S98>/Delay1'

      Parker_Velocity_Control_B.Subtract_o =
        Parker_Velocity_Control_B.UDPReceive_o1[1] -
        Parker_Velocity_Control_DW.Delay1_DSTATE_kv;

      // If: '<S98>/if we went through a "step"' incorporates:
      //   Inport: '<S113>/In1'

      if (Parker_Velocity_Control_B.Subtract_o != 0.0) {
        // Outputs for IfAction SubSystem: '<S98>/Hold this value' incorporates:
        //   ActionPort: '<S113>/Action Port'

        Parker_Velocity_Control_B.In1_ky = Parker_Velocity_Control_B.Subtract_o;

        // End of Outputs for SubSystem: '<S98>/Hold this value'
      }

      // End of If: '<S98>/if we went through a "step"'

      // Gain: '<S98>/divide by delta T'
      Parker_Velocity_Control_B.dividebydeltaT_j = 1.0 /
        Parker_Velocity_Control_P.baseRate * Parker_Velocity_Control_B.In1_ky;

      // DataStoreWrite: '<S97>/RED_Px2'
      Parker_Velocity_Control_DW.RED_Vx =
        Parker_Velocity_Control_B.dividebydeltaT_j;

      // DataStoreWrite: '<S97>/RED_Px20'
      Parker_Velocity_Control_DW.BLUE_Rz =
        Parker_Velocity_Control_B.UDPReceive_o1[15];

      // DataStoreWrite: '<S97>/RED_Px3'
      Parker_Velocity_Control_DW.BLACK_Vx =
        Parker_Velocity_Control_B.dividebydeltaT;

      // DataStoreWrite: '<S97>/RED_Px4'
      Parker_Velocity_Control_DW.BLACK_Px =
        Parker_Velocity_Control_B.UDPReceive_o1[4];

      // DataStoreWrite: '<S97>/RED_Px5'
      Parker_Velocity_Control_DW.BLACK_Py =
        Parker_Velocity_Control_B.UDPReceive_o1[5];

      // DataStoreWrite: '<S97>/RED_Px6'
      Parker_Velocity_Control_DW.BLACK_Rz =
        Parker_Velocity_Control_B.UDPReceive_o1[6];

      // Delay: '<S110>/Delay1'
      if (Parker_Velocity_Control_DW.icLoad_df != 0) {
        Parker_Velocity_Control_DW.Delay1_DSTATE_lc =
          Parker_Velocity_Control_B.dividebydeltaT_j;
      }

      // Sum: '<S110>/Sum6' incorporates:
      //   Delay: '<S110>/Delay1'

      Parker_Velocity_Control_B.Subtract_o =
        Parker_Velocity_Control_B.dividebydeltaT_j -
        Parker_Velocity_Control_DW.Delay1_DSTATE_lc;

      // If: '<S110>/if we went through a "step"' incorporates:
      //   Inport: '<S125>/In1'

      if (Parker_Velocity_Control_B.Subtract_o != 0.0) {
        // Outputs for IfAction SubSystem: '<S110>/Hold this value' incorporates:
        //   ActionPort: '<S125>/Action Port'

        Parker_Velocity_Control_B.In1_fo = Parker_Velocity_Control_B.Subtract_o;

        // End of Outputs for SubSystem: '<S110>/Hold this value'
      }

      // End of If: '<S110>/if we went through a "step"'

      // Gain: '<S110>/divide by delta T' incorporates:
      //   DataStoreWrite: '<S97>/RED_Px7'

      Parker_Velocity_Control_DW.RED_Ax = 1.0 /
        Parker_Velocity_Control_P.baseRate * Parker_Velocity_Control_B.In1_fo;

      // Delay: '<S105>/Delay1'
      if (Parker_Velocity_Control_DW.icLoad_pj != 0) {
        Parker_Velocity_Control_DW.Delay1_DSTATE_cd =
          Parker_Velocity_Control_B.UDPReceive_o1[2];
      }

      // Sum: '<S105>/Sum6' incorporates:
      //   Delay: '<S105>/Delay1'

      Parker_Velocity_Control_B.Subtract_o =
        Parker_Velocity_Control_B.UDPReceive_o1[2] -
        Parker_Velocity_Control_DW.Delay1_DSTATE_cd;

      // If: '<S105>/if we went through a "step"' incorporates:
      //   Inport: '<S120>/In1'

      if (Parker_Velocity_Control_B.Subtract_o != 0.0) {
        // Outputs for IfAction SubSystem: '<S105>/Hold this value' incorporates:
        //   ActionPort: '<S120>/Action Port'

        Parker_Velocity_Control_B.In1_f0 = Parker_Velocity_Control_B.Subtract_o;

        // End of Outputs for SubSystem: '<S105>/Hold this value'
      }

      // End of If: '<S105>/if we went through a "step"'

      // Gain: '<S105>/divide by delta T'
      Parker_Velocity_Control_B.dividebydeltaT_d2 = 1.0 /
        Parker_Velocity_Control_P.baseRate * Parker_Velocity_Control_B.In1_f0;

      // Delay: '<S111>/Delay1'
      if (Parker_Velocity_Control_DW.icLoad_ap != 0) {
        Parker_Velocity_Control_DW.Delay1_DSTATE_i0 =
          Parker_Velocity_Control_B.dividebydeltaT_d2;
      }

      // Sum: '<S111>/Sum6' incorporates:
      //   Delay: '<S111>/Delay1'

      Parker_Velocity_Control_B.Subtract_o =
        Parker_Velocity_Control_B.dividebydeltaT_d2 -
        Parker_Velocity_Control_DW.Delay1_DSTATE_i0;

      // If: '<S111>/if we went through a "step"' incorporates:
      //   Inport: '<S126>/In1'

      if (Parker_Velocity_Control_B.Subtract_o != 0.0) {
        // Outputs for IfAction SubSystem: '<S111>/Hold this value' incorporates:
        //   ActionPort: '<S126>/Action Port'

        Parker_Velocity_Control_B.In1_db = Parker_Velocity_Control_B.Subtract_o;

        // End of Outputs for SubSystem: '<S111>/Hold this value'
      }

      // End of If: '<S111>/if we went through a "step"'

      // Gain: '<S111>/divide by delta T' incorporates:
      //   DataStoreWrite: '<S97>/RED_Px8'

      Parker_Velocity_Control_DW.RED_Ay = 1.0 /
        Parker_Velocity_Control_P.baseRate * Parker_Velocity_Control_B.In1_db;

      // S-Function (sdspunwrap2): '<S97>/Unwrap'
      if (Parker_Velocity_Control_DW.Unwrap_FirstStep) {
        Parker_Velocity_Control_B.i = 0;
        while (Parker_Velocity_Control_B.i < 1) {
          Parker_Velocity_Control_DW.Unwrap_Prev =
            Parker_Velocity_Control_B.UDPReceive_o1[3];
          Parker_Velocity_Control_B.i++;
        }

        Parker_Velocity_Control_DW.Unwrap_FirstStep = false;
      }

      Parker_Velocity_Control_B.i = 0;
      while (Parker_Velocity_Control_B.i < 1) {
        Parker_Velocity_Control_B.Subtract_o =
          Parker_Velocity_Control_DW.Unwrap_Cumsum;
        Parker_Velocity_Control_B.Sum6_l =
          Parker_Velocity_Control_B.UDPReceive_o1[3] -
          Parker_Velocity_Control_DW.Unwrap_Prev;
        Parker_Velocity_Control_B.Sum6_d = Parker_Velocity_Control_B.Sum6_l -
          floor((Parker_Velocity_Control_B.Sum6_l + 3.1415926535897931) /
                6.2831853071795862) * 6.2831853071795862;
        if ((Parker_Velocity_Control_B.Sum6_d == -3.1415926535897931) &&
            (Parker_Velocity_Control_B.Sum6_l > 0.0)) {
          Parker_Velocity_Control_B.Sum6_d = 3.1415926535897931;
        }

        Parker_Velocity_Control_B.Sum6_l = Parker_Velocity_Control_B.Sum6_d -
          Parker_Velocity_Control_B.Sum6_l;
        if (fabs(Parker_Velocity_Control_B.Sum6_l) > 3.1415926535897931) {
          Parker_Velocity_Control_B.Subtract_o =
            Parker_Velocity_Control_DW.Unwrap_Cumsum +
            Parker_Velocity_Control_B.Sum6_l;
        }

        Parker_Velocity_Control_DW.Unwrap_Prev =
          Parker_Velocity_Control_B.UDPReceive_o1[3];
        Parker_Velocity_Control_B.Unwrap =
          Parker_Velocity_Control_B.UDPReceive_o1[3] +
          Parker_Velocity_Control_B.Subtract_o;
        Parker_Velocity_Control_DW.Unwrap_Cumsum =
          Parker_Velocity_Control_B.Subtract_o;
        Parker_Velocity_Control_B.i++;
      }

      // End of S-Function (sdspunwrap2): '<S97>/Unwrap'

      // Delay: '<S106>/Delay1'
      if (Parker_Velocity_Control_DW.icLoad_hr != 0) {
        Parker_Velocity_Control_DW.Delay1_DSTATE_jx =
          Parker_Velocity_Control_B.Unwrap;
      }

      // Sum: '<S106>/Sum6' incorporates:
      //   Delay: '<S106>/Delay1'

      Parker_Velocity_Control_B.Subtract_o = Parker_Velocity_Control_B.Unwrap -
        Parker_Velocity_Control_DW.Delay1_DSTATE_jx;

      // If: '<S106>/if we went through a "step"' incorporates:
      //   Inport: '<S121>/In1'

      if (Parker_Velocity_Control_B.Subtract_o != 0.0) {
        // Outputs for IfAction SubSystem: '<S106>/Hold this value' incorporates:
        //   ActionPort: '<S121>/Action Port'

        Parker_Velocity_Control_B.In1_lb = Parker_Velocity_Control_B.Subtract_o;

        // End of Outputs for SubSystem: '<S106>/Hold this value'
      }

      // End of If: '<S106>/if we went through a "step"'

      // Gain: '<S106>/divide by delta T'
      Parker_Velocity_Control_B.dividebydeltaT_l = 1.0 /
        Parker_Velocity_Control_P.baseRate * Parker_Velocity_Control_B.In1_lb;

      // Delay: '<S112>/Delay1'
      if (Parker_Velocity_Control_DW.icLoad_al != 0) {
        Parker_Velocity_Control_DW.Delay1_DSTATE_ml =
          Parker_Velocity_Control_B.dividebydeltaT_l;
      }

      // Sum: '<S112>/Sum6' incorporates:
      //   Delay: '<S112>/Delay1'

      Parker_Velocity_Control_B.Subtract_o =
        Parker_Velocity_Control_B.dividebydeltaT_l -
        Parker_Velocity_Control_DW.Delay1_DSTATE_ml;

      // If: '<S112>/if we went through a "step"' incorporates:
      //   Inport: '<S127>/In1'

      if (Parker_Velocity_Control_B.Subtract_o != 0.0) {
        // Outputs for IfAction SubSystem: '<S112>/Hold this value' incorporates:
        //   ActionPort: '<S127>/Action Port'

        Parker_Velocity_Control_B.In1_dn = Parker_Velocity_Control_B.Subtract_o;

        // End of Outputs for SubSystem: '<S112>/Hold this value'
      }

      // End of If: '<S112>/if we went through a "step"'

      // Gain: '<S112>/divide by delta T' incorporates:
      //   DataStoreWrite: '<S97>/RED_Px9'

      Parker_Velocity_Control_DW.RED_RzDD = 1.0 /
        Parker_Velocity_Control_P.baseRate * Parker_Velocity_Control_B.In1_dn;

      // DataStoreWrite: '<S97>/RED_Py1'
      Parker_Velocity_Control_DW.RED_Py =
        Parker_Velocity_Control_B.UDPReceive_o1[2];

      // DataStoreWrite: '<S97>/RED_Py2'
      Parker_Velocity_Control_DW.RED_Vy =
        Parker_Velocity_Control_B.dividebydeltaT_d2;

      // DataStoreWrite: '<S97>/RED_Py3'
      Parker_Velocity_Control_DW.BLACK_Vy =
        Parker_Velocity_Control_B.dividebydeltaT_k;

      // Delay: '<S103>/Delay1'
      if (Parker_Velocity_Control_DW.icLoad_dt != 0) {
        Parker_Velocity_Control_DW.Delay1_DSTATE_ic =
          Parker_Velocity_Control_B.UDPReceive_o1[14];
      }

      // Sum: '<S103>/Sum6' incorporates:
      //   Delay: '<S103>/Delay1'

      Parker_Velocity_Control_B.Subtract_o =
        Parker_Velocity_Control_B.UDPReceive_o1[14] -
        Parker_Velocity_Control_DW.Delay1_DSTATE_ic;

      // If: '<S103>/if we went through a "step"' incorporates:
      //   Inport: '<S118>/In1'

      if (Parker_Velocity_Control_B.Subtract_o != 0.0) {
        // Outputs for IfAction SubSystem: '<S103>/Hold this value' incorporates:
        //   ActionPort: '<S118>/Action Port'

        Parker_Velocity_Control_B.In1_as = Parker_Velocity_Control_B.Subtract_o;

        // End of Outputs for SubSystem: '<S103>/Hold this value'
      }

      // End of If: '<S103>/if we went through a "step"'

      // Gain: '<S103>/divide by delta T' incorporates:
      //   DataStoreWrite: '<S97>/RED_Py4'

      Parker_Velocity_Control_DW.BLUE_Vy = 1.0 /
        Parker_Velocity_Control_P.baseRate * Parker_Velocity_Control_B.In1_as;

      // DataStoreWrite: '<S97>/RED_Rz1'
      Parker_Velocity_Control_DW.RED_Rz =
        Parker_Velocity_Control_B.UDPReceive_o1[3];

      // DataStoreWrite: '<S97>/RED_Rz2'
      Parker_Velocity_Control_DW.RED_RzD =
        Parker_Velocity_Control_B.dividebydeltaT_l;

      // DataStoreWrite: '<S97>/RED_Rz3'
      Parker_Velocity_Control_DW.BLACK_RzD =
        Parker_Velocity_Control_B.dividebydeltaT_d;

      // S-Function (sdspunwrap2): '<S97>/Unwrap2'
      if (Parker_Velocity_Control_DW.Unwrap2_FirstStep) {
        Parker_Velocity_Control_B.i = 0;
        while (Parker_Velocity_Control_B.i < 1) {
          Parker_Velocity_Control_DW.Unwrap2_Prev =
            Parker_Velocity_Control_B.UDPReceive_o1[15];
          Parker_Velocity_Control_B.i++;
        }

        Parker_Velocity_Control_DW.Unwrap2_FirstStep = false;
      }

      Parker_Velocity_Control_B.i = 0;
      while (Parker_Velocity_Control_B.i < 1) {
        Parker_Velocity_Control_B.Subtract_o =
          Parker_Velocity_Control_DW.Unwrap2_Cumsum;
        Parker_Velocity_Control_B.Sum6_l =
          Parker_Velocity_Control_B.UDPReceive_o1[15] -
          Parker_Velocity_Control_DW.Unwrap2_Prev;
        Parker_Velocity_Control_B.Sum6_d = Parker_Velocity_Control_B.Sum6_l -
          floor((Parker_Velocity_Control_B.Sum6_l + 3.1415926535897931) /
                6.2831853071795862) * 6.2831853071795862;
        if ((Parker_Velocity_Control_B.Sum6_d == -3.1415926535897931) &&
            (Parker_Velocity_Control_B.Sum6_l > 0.0)) {
          Parker_Velocity_Control_B.Sum6_d = 3.1415926535897931;
        }

        Parker_Velocity_Control_B.Sum6_l = Parker_Velocity_Control_B.Sum6_d -
          Parker_Velocity_Control_B.Sum6_l;
        if (fabs(Parker_Velocity_Control_B.Sum6_l) > 3.1415926535897931) {
          Parker_Velocity_Control_B.Subtract_o =
            Parker_Velocity_Control_DW.Unwrap2_Cumsum +
            Parker_Velocity_Control_B.Sum6_l;
        }

        Parker_Velocity_Control_DW.Unwrap2_Prev =
          Parker_Velocity_Control_B.UDPReceive_o1[15];
        Parker_Velocity_Control_B.Unwrap2 =
          Parker_Velocity_Control_B.UDPReceive_o1[15] +
          Parker_Velocity_Control_B.Subtract_o;
        Parker_Velocity_Control_DW.Unwrap2_Cumsum =
          Parker_Velocity_Control_B.Subtract_o;
        Parker_Velocity_Control_B.i++;
      }

      // End of S-Function (sdspunwrap2): '<S97>/Unwrap2'

      // Delay: '<S104>/Delay1'
      if (Parker_Velocity_Control_DW.icLoad_l1b != 0) {
        Parker_Velocity_Control_DW.Delay1_DSTATE_a =
          Parker_Velocity_Control_B.Unwrap2;
      }

      // Sum: '<S104>/Sum6' incorporates:
      //   Delay: '<S104>/Delay1'

      Parker_Velocity_Control_B.Subtract_o = Parker_Velocity_Control_B.Unwrap2 -
        Parker_Velocity_Control_DW.Delay1_DSTATE_a;

      // If: '<S104>/if we went through a "step"' incorporates:
      //   Inport: '<S119>/In1'

      if (Parker_Velocity_Control_B.Subtract_o != 0.0) {
        // Outputs for IfAction SubSystem: '<S104>/Hold this value' incorporates:
        //   ActionPort: '<S119>/Action Port'

        Parker_Velocity_Control_B.In1_lu = Parker_Velocity_Control_B.Subtract_o;

        // End of Outputs for SubSystem: '<S104>/Hold this value'
      }

      // End of If: '<S104>/if we went through a "step"'

      // Gain: '<S104>/divide by delta T' incorporates:
      //   DataStoreWrite: '<S97>/RED_Rz4'

      Parker_Velocity_Control_DW.BLUE_RzD = 1.0 /
        Parker_Velocity_Control_P.baseRate * Parker_Velocity_Control_B.In1_lu;

      // End of Outputs for SubSystem: '<S6>/Use Hardware to Obtain States'
    } else {
      if (Parker_Velocity_Control_P.simMode == 1.0) {
        Parker_Velocity_Control_B.rtAction = 1;

        // Outputs for IfAction SubSystem: '<S6>/Initialize Universal Time (Simulation)' incorporates:
        //   ActionPort: '<S95>/Action Port'

        // Clock: '<S95>/Set Universal Time (If this is a simulation)' incorporates:
        //   DataStoreWrite: '<S95>/Universal_Time'

        Parker_Velocity_Control_DW.Univ_Time =
          Parker_Velocity_Control_M->Timing.t[0];

        // End of Outputs for SubSystem: '<S6>/Initialize Universal Time (Simulation)' 
      }
    }

    Parker_Velocity_Control_DW.Ifperforminganexperimentgrabthe =
      Parker_Velocity_Control_B.rtAction;

    // End of If: '<S6>/If performing an experiment, grab the PhaseSpace data. Otherwise, use a clock to set time in SIM.' 

    // If: '<S8>/This IF block determines whether or not to run the exp code' incorporates:
    //   Constant: '<S129>/Constant'

    if (Parker_Velocity_Control_P.simMode == 0.0) {
      // Outputs for IfAction SubSystem: '<S8>/Change Behavior' incorporates:
      //   ActionPort: '<S128>/Action Port'

      // MATLABSystem: '<S128>/Read Joint Positions using  Dynamixel Encoders' incorporates:
      //   DataStoreWrite: '<S128>/Data Store Write3'
      //   DataStoreWrite: '<S128>/Data Store Write4'
      //   DataStoreWrite: '<S128>/Data Store Write5'

      if (Parker_Velocity_Control_DW.obj_c.SampleTime !=
          Parker_Velocity_Control_P.baseRate) {
        if (((!rtIsInf(Parker_Velocity_Control_P.baseRate)) && (!rtIsNaN
              (Parker_Velocity_Control_P.baseRate))) || rtIsInf
            (Parker_Velocity_Control_P.baseRate)) {
          Parker_Velocity_Control_B.sampleTime =
            Parker_Velocity_Control_P.baseRate;
        }

        Parker_Velocity_Control_DW.obj_c.SampleTime =
          Parker_Velocity_Control_B.sampleTime;
      }

      Parker_Velocity_Control_B.sampleTime = 0.0;
      Parker_Velocity_Control_B.Subtract_o = 0.0;
      Parker_Velocity_Control_B.Sum6_l = 0.0;
      Parker_Velocity_Control_DW.RED_RzD_Shoulder = 0.0;
      Parker_Velocity_Control_DW.RED_RzD_Elbow = 0.0;
      Parker_Velocity_Control_DW.RED_RzD_Wrist = 0.0;
      read_dynamixel_position(&Parker_Velocity_Control_B.sampleTime,
        &Parker_Velocity_Control_B.Subtract_o, &Parker_Velocity_Control_B.Sum6_l,
        &Parker_Velocity_Control_DW.RED_RzD_Shoulder,
        &Parker_Velocity_Control_DW.RED_RzD_Elbow,
        &Parker_Velocity_Control_DW.RED_RzD_Wrist);

      // MATLAB Function: '<S130>/Correct Dropouts' incorporates:
      //   Delay: '<S130>/Delay1'
      //   Delay: '<S130>/Delay2'
      //   Delay: '<S130>/Delay3'
      //   Delay: '<S130>/Delay4'
      //   Delay: '<S130>/Delay5'
      //   MATLABSystem: '<S128>/Read Joint Positions using  Dynamixel Encoders'

      Parker_Velo_CorrectDropouts(Parker_Velocity_Control_B.sampleTime,
        Parker_Velocity_Control_DW.Delay1_DSTATE_jp,
        &Parker_Velocity_Control_B.sf_CorrectDropouts);

      // DataStoreWrite: '<S128>/Data Store Write'
      Parker_Velocity_Control_DW.RED_Rz_Shoulder =
        Parker_Velocity_Control_B.sf_CorrectDropouts.FixedInput;

      // MATLAB Function: '<S131>/Correct Dropouts' incorporates:
      //   Delay: '<S131>/Delay1'
      //   Delay: '<S131>/Delay2'
      //   Delay: '<S131>/Delay3'
      //   Delay: '<S131>/Delay4'
      //   Delay: '<S131>/Delay5'
      //   MATLABSystem: '<S128>/Read Joint Positions using  Dynamixel Encoders'

      Parker_Velo_CorrectDropouts(Parker_Velocity_Control_B.Subtract_o,
        Parker_Velocity_Control_DW.Delay1_DSTATE_n5,
        &Parker_Velocity_Control_B.sf_CorrectDropouts_b);

      // DataStoreWrite: '<S128>/Data Store Write1'
      Parker_Velocity_Control_DW.RED_Rz_Elbow =
        Parker_Velocity_Control_B.sf_CorrectDropouts_b.FixedInput;

      // MATLAB Function: '<S132>/Correct Dropouts' incorporates:
      //   Delay: '<S132>/Delay1'
      //   Delay: '<S132>/Delay2'
      //   Delay: '<S132>/Delay3'
      //   Delay: '<S132>/Delay4'
      //   Delay: '<S132>/Delay5'
      //   MATLABSystem: '<S128>/Read Joint Positions using  Dynamixel Encoders'

      Parker_Velo_CorrectDropouts(Parker_Velocity_Control_B.Sum6_l,
        Parker_Velocity_Control_DW.Delay1_DSTATE_ny,
        &Parker_Velocity_Control_B.sf_CorrectDropouts_h);

      // DataStoreWrite: '<S128>/Data Store Write2'
      Parker_Velocity_Control_DW.RED_Rz_Wrist =
        Parker_Velocity_Control_B.sf_CorrectDropouts_h.FixedInput;

      // Update for Delay: '<S130>/Delay1' incorporates:
      //   MATLABSystem: '<S128>/Read Joint Positions using  Dynamixel Encoders'

      Parker_Velocity_Control_DW.Delay1_DSTATE_jp =
        Parker_Velocity_Control_B.sampleTime;

      // Update for Delay: '<S130>/Delay2' incorporates:
      //   MATLABSystem: '<S128>/Read Joint Positions using  Dynamixel Encoders'

      Parker_Velocity_Control_B.i = 0;
      while (Parker_Velocity_Control_B.i < 1) {
        Parker_Velocity_Control_DW.Delay2_DSTATE[Parker_Velocity_Control_B.i] =
          Parker_Velocity_Control_DW.Delay2_DSTATE[Parker_Velocity_Control_B.i +
          1];
        Parker_Velocity_Control_B.i++;
      }

      Parker_Velocity_Control_DW.Delay2_DSTATE[1] =
        Parker_Velocity_Control_B.sampleTime;

      // End of Update for Delay: '<S130>/Delay2'

      // Update for Delay: '<S130>/Delay5' incorporates:
      //   MATLABSystem: '<S128>/Read Joint Positions using  Dynamixel Encoders'

      Parker_Velocity_Control_B.i = 0;
      while (Parker_Velocity_Control_B.i < 2) {
        Parker_Velocity_Control_DW.Delay5_DSTATE[Parker_Velocity_Control_B.i] =
          Parker_Velocity_Control_DW.Delay5_DSTATE[Parker_Velocity_Control_B.i +
          1];
        Parker_Velocity_Control_B.i++;
      }

      Parker_Velocity_Control_DW.Delay5_DSTATE[2] =
        Parker_Velocity_Control_B.sampleTime;

      // End of Update for Delay: '<S130>/Delay5'

      // Update for Delay: '<S130>/Delay3' incorporates:
      //   MATLABSystem: '<S128>/Read Joint Positions using  Dynamixel Encoders'

      Parker_Velocity_Control_B.i = 0;
      while (Parker_Velocity_Control_B.i < 3) {
        Parker_Velocity_Control_DW.Delay3_DSTATE[Parker_Velocity_Control_B.i] =
          Parker_Velocity_Control_DW.Delay3_DSTATE[Parker_Velocity_Control_B.i +
          1];
        Parker_Velocity_Control_B.i++;
      }

      Parker_Velocity_Control_DW.Delay3_DSTATE[3] =
        Parker_Velocity_Control_B.sampleTime;

      // End of Update for Delay: '<S130>/Delay3'

      // Update for Delay: '<S130>/Delay4' incorporates:
      //   MATLABSystem: '<S128>/Read Joint Positions using  Dynamixel Encoders'

      Parker_Velocity_Control_B.i = 0;
      while (Parker_Velocity_Control_B.i < 4) {
        Parker_Velocity_Control_DW.Delay4_DSTATE[Parker_Velocity_Control_B.i] =
          Parker_Velocity_Control_DW.Delay4_DSTATE[Parker_Velocity_Control_B.i +
          1];
        Parker_Velocity_Control_B.i++;
      }

      Parker_Velocity_Control_DW.Delay4_DSTATE[4] =
        Parker_Velocity_Control_B.sampleTime;

      // End of Update for Delay: '<S130>/Delay4'

      // Update for Delay: '<S131>/Delay1' incorporates:
      //   MATLABSystem: '<S128>/Read Joint Positions using  Dynamixel Encoders'

      Parker_Velocity_Control_DW.Delay1_DSTATE_n5 =
        Parker_Velocity_Control_B.Subtract_o;

      // Update for Delay: '<S131>/Delay2' incorporates:
      //   MATLABSystem: '<S128>/Read Joint Positions using  Dynamixel Encoders'

      Parker_Velocity_Control_B.i = 0;
      while (Parker_Velocity_Control_B.i < 1) {
        Parker_Velocity_Control_DW.Delay2_DSTATE_l[Parker_Velocity_Control_B.i] =
          Parker_Velocity_Control_DW.Delay2_DSTATE_l[Parker_Velocity_Control_B.i
          + 1];
        Parker_Velocity_Control_B.i++;
      }

      Parker_Velocity_Control_DW.Delay2_DSTATE_l[1] =
        Parker_Velocity_Control_B.Subtract_o;

      // End of Update for Delay: '<S131>/Delay2'

      // Update for Delay: '<S131>/Delay5' incorporates:
      //   MATLABSystem: '<S128>/Read Joint Positions using  Dynamixel Encoders'

      Parker_Velocity_Control_B.i = 0;
      while (Parker_Velocity_Control_B.i < 2) {
        Parker_Velocity_Control_DW.Delay5_DSTATE_b[Parker_Velocity_Control_B.i] =
          Parker_Velocity_Control_DW.Delay5_DSTATE_b[Parker_Velocity_Control_B.i
          + 1];
        Parker_Velocity_Control_B.i++;
      }

      Parker_Velocity_Control_DW.Delay5_DSTATE_b[2] =
        Parker_Velocity_Control_B.Subtract_o;

      // End of Update for Delay: '<S131>/Delay5'

      // Update for Delay: '<S131>/Delay3' incorporates:
      //   MATLABSystem: '<S128>/Read Joint Positions using  Dynamixel Encoders'

      Parker_Velocity_Control_B.i = 0;
      while (Parker_Velocity_Control_B.i < 3) {
        Parker_Velocity_Control_DW.Delay3_DSTATE_o[Parker_Velocity_Control_B.i] =
          Parker_Velocity_Control_DW.Delay3_DSTATE_o[Parker_Velocity_Control_B.i
          + 1];
        Parker_Velocity_Control_B.i++;
      }

      Parker_Velocity_Control_DW.Delay3_DSTATE_o[3] =
        Parker_Velocity_Control_B.Subtract_o;

      // End of Update for Delay: '<S131>/Delay3'

      // Update for Delay: '<S131>/Delay4' incorporates:
      //   MATLABSystem: '<S128>/Read Joint Positions using  Dynamixel Encoders'

      Parker_Velocity_Control_B.i = 0;
      while (Parker_Velocity_Control_B.i < 4) {
        Parker_Velocity_Control_DW.Delay4_DSTATE_c[Parker_Velocity_Control_B.i] =
          Parker_Velocity_Control_DW.Delay4_DSTATE_c[Parker_Velocity_Control_B.i
          + 1];
        Parker_Velocity_Control_B.i++;
      }

      Parker_Velocity_Control_DW.Delay4_DSTATE_c[4] =
        Parker_Velocity_Control_B.Subtract_o;

      // End of Update for Delay: '<S131>/Delay4'

      // Update for Delay: '<S132>/Delay1' incorporates:
      //   MATLABSystem: '<S128>/Read Joint Positions using  Dynamixel Encoders'

      Parker_Velocity_Control_DW.Delay1_DSTATE_ny =
        Parker_Velocity_Control_B.Sum6_l;

      // Update for Delay: '<S132>/Delay2' incorporates:
      //   MATLABSystem: '<S128>/Read Joint Positions using  Dynamixel Encoders'

      Parker_Velocity_Control_B.i = 0;
      while (Parker_Velocity_Control_B.i < 1) {
        Parker_Velocity_Control_DW.Delay2_DSTATE_m[Parker_Velocity_Control_B.i] =
          Parker_Velocity_Control_DW.Delay2_DSTATE_m[Parker_Velocity_Control_B.i
          + 1];
        Parker_Velocity_Control_B.i++;
      }

      Parker_Velocity_Control_DW.Delay2_DSTATE_m[1] =
        Parker_Velocity_Control_B.Sum6_l;

      // End of Update for Delay: '<S132>/Delay2'

      // Update for Delay: '<S132>/Delay5' incorporates:
      //   MATLABSystem: '<S128>/Read Joint Positions using  Dynamixel Encoders'

      Parker_Velocity_Control_B.i = 0;
      while (Parker_Velocity_Control_B.i < 2) {
        Parker_Velocity_Control_DW.Delay5_DSTATE_n[Parker_Velocity_Control_B.i] =
          Parker_Velocity_Control_DW.Delay5_DSTATE_n[Parker_Velocity_Control_B.i
          + 1];
        Parker_Velocity_Control_B.i++;
      }

      Parker_Velocity_Control_DW.Delay5_DSTATE_n[2] =
        Parker_Velocity_Control_B.Sum6_l;

      // End of Update for Delay: '<S132>/Delay5'

      // Update for Delay: '<S132>/Delay3' incorporates:
      //   MATLABSystem: '<S128>/Read Joint Positions using  Dynamixel Encoders'

      Parker_Velocity_Control_B.i = 0;
      while (Parker_Velocity_Control_B.i < 3) {
        Parker_Velocity_Control_DW.Delay3_DSTATE_h[Parker_Velocity_Control_B.i] =
          Parker_Velocity_Control_DW.Delay3_DSTATE_h[Parker_Velocity_Control_B.i
          + 1];
        Parker_Velocity_Control_B.i++;
      }

      Parker_Velocity_Control_DW.Delay3_DSTATE_h[3] =
        Parker_Velocity_Control_B.Sum6_l;

      // End of Update for Delay: '<S132>/Delay3'

      // Update for Delay: '<S132>/Delay4' incorporates:
      //   MATLABSystem: '<S128>/Read Joint Positions using  Dynamixel Encoders'

      Parker_Velocity_Control_B.i = 0;
      while (Parker_Velocity_Control_B.i < 4) {
        Parker_Velocity_Control_DW.Delay4_DSTATE_k[Parker_Velocity_Control_B.i] =
          Parker_Velocity_Control_DW.Delay4_DSTATE_k[Parker_Velocity_Control_B.i
          + 1];
        Parker_Velocity_Control_B.i++;
      }

      Parker_Velocity_Control_DW.Delay4_DSTATE_k[4] =
        Parker_Velocity_Control_B.Sum6_l;

      // End of Update for Delay: '<S132>/Delay4'
      // End of Outputs for SubSystem: '<S8>/Change Behavior'
    }

    // End of If: '<S8>/This IF block determines whether or not to run the exp code' 

    // Clock: '<S18>/Set Universal Time (If this is a simulation)'
    Parker_Velocity_Control_B.SetUniversalTimeIfthisisasimula =
      Parker_Velocity_Control_M->Timing.t[0];

    // MATLABSystem: '<Root>/Initialization Block for Speed Control'
    if (Parker_Velocity_Control_DW.obj.P_GAIN !=
        Parker_Velocity_Control_P.InitializationBlockforSpeedCont) {
      Parker_Velocity_Control_DW.obj.P_GAIN =
        Parker_Velocity_Control_P.InitializationBlockforSpeedCont;
    }

    if (Parker_Velocity_Control_DW.obj.I_GAIN !=
        Parker_Velocity_Control_P.InitializationBlockforSpeedCo_d) {
      Parker_Velocity_Control_DW.obj.I_GAIN =
        Parker_Velocity_Control_P.InitializationBlockforSpeedCo_d;
    }

    if (Parker_Velocity_Control_DW.obj.VELOCITY_LIMIT !=
        Parker_Velocity_Control_P.InitializationBlockforSpeedCo_k) {
      Parker_Velocity_Control_DW.obj.VELOCITY_LIMIT =
        Parker_Velocity_Control_P.InitializationBlockforSpeedCo_k;
    }

    if (Parker_Velocity_Control_DW.obj.ACCELERATION_TIME !=
        Parker_Velocity_Control_P.InitializationBlockforSpeedCo_g) {
      Parker_Velocity_Control_DW.obj.ACCELERATION_TIME =
        Parker_Velocity_Control_P.InitializationBlockforSpeedCo_g;
    }

    // End of MATLABSystem: '<Root>/Initialization Block for Speed Control'
  }

  // Matfile logging
  rt_UpdateTXYLogVars(Parker_Velocity_Control_M->rtwLogInfo,
                      (Parker_Velocity_Control_M->Timing.t));

  {
    char_T *sErr;

    // Update for If: '<S6>/If performing an experiment, grab the PhaseSpace data. Otherwise, use a clock to set time in SIM.' 
    if (Parker_Velocity_Control_DW.Ifperforminganexperimentgrabthe == 0) {
      // Update for IfAction SubSystem: '<S6>/Use Hardware to Obtain States' incorporates:
      //   ActionPort: '<S97>/Action Port'

      // Update for Delay: '<S107>/Delay1'
      Parker_Velocity_Control_DW.icLoad_c = 0U;
      Parker_Velocity_Control_DW.Delay1_DSTATE_hi =
        Parker_Velocity_Control_B.UDPReceive_o1[4];

      // Update for Delay: '<S99>/Delay1'
      Parker_Velocity_Control_DW.icLoad_h0 = 0U;
      Parker_Velocity_Control_DW.Delay1_DSTATE_df =
        Parker_Velocity_Control_B.dividebydeltaT;

      // Update for Delay: '<S108>/Delay1'
      Parker_Velocity_Control_DW.icLoad_kt = 0U;
      Parker_Velocity_Control_DW.Delay1_DSTATE_ft =
        Parker_Velocity_Control_B.UDPReceive_o1[5];

      // Update for Delay: '<S100>/Delay1'
      Parker_Velocity_Control_DW.icLoad_ktm = 0U;
      Parker_Velocity_Control_DW.Delay1_DSTATE_hz =
        Parker_Velocity_Control_B.dividebydeltaT_k;

      // Update for Delay: '<S109>/Delay1'
      Parker_Velocity_Control_DW.icLoad_gn = 0U;
      Parker_Velocity_Control_DW.Delay1_DSTATE_mt =
        Parker_Velocity_Control_B.Unwrap1;

      // Update for Delay: '<S101>/Delay1'
      Parker_Velocity_Control_DW.icLoad_pz = 0U;
      Parker_Velocity_Control_DW.Delay1_DSTATE_lf =
        Parker_Velocity_Control_B.dividebydeltaT_d;

      // Update for Delay: '<S102>/Delay1'
      Parker_Velocity_Control_DW.icLoad_fy = 0U;
      Parker_Velocity_Control_DW.Delay1_DSTATE_cs =
        Parker_Velocity_Control_B.UDPReceive_o1[13];

      // Update for Delay: '<S98>/Delay1'
      Parker_Velocity_Control_DW.icLoad_ea = 0U;
      Parker_Velocity_Control_DW.Delay1_DSTATE_kv =
        Parker_Velocity_Control_B.UDPReceive_o1[1];

      // Update for Delay: '<S110>/Delay1'
      Parker_Velocity_Control_DW.icLoad_df = 0U;
      Parker_Velocity_Control_DW.Delay1_DSTATE_lc =
        Parker_Velocity_Control_B.dividebydeltaT_j;

      // Update for Delay: '<S105>/Delay1'
      Parker_Velocity_Control_DW.icLoad_pj = 0U;
      Parker_Velocity_Control_DW.Delay1_DSTATE_cd =
        Parker_Velocity_Control_B.UDPReceive_o1[2];

      // Update for Delay: '<S111>/Delay1'
      Parker_Velocity_Control_DW.icLoad_ap = 0U;
      Parker_Velocity_Control_DW.Delay1_DSTATE_i0 =
        Parker_Velocity_Control_B.dividebydeltaT_d2;

      // Update for Delay: '<S106>/Delay1'
      Parker_Velocity_Control_DW.icLoad_hr = 0U;
      Parker_Velocity_Control_DW.Delay1_DSTATE_jx =
        Parker_Velocity_Control_B.Unwrap;

      // Update for Delay: '<S112>/Delay1'
      Parker_Velocity_Control_DW.icLoad_al = 0U;
      Parker_Velocity_Control_DW.Delay1_DSTATE_ml =
        Parker_Velocity_Control_B.dividebydeltaT_l;

      // Update for Delay: '<S103>/Delay1'
      Parker_Velocity_Control_DW.icLoad_dt = 0U;
      Parker_Velocity_Control_DW.Delay1_DSTATE_ic =
        Parker_Velocity_Control_B.UDPReceive_o1[14];

      // Update for Delay: '<S104>/Delay1'
      Parker_Velocity_Control_DW.icLoad_l1b = 0U;
      Parker_Velocity_Control_DW.Delay1_DSTATE_a =
        Parker_Velocity_Control_B.Unwrap2;

      // End of Update for SubSystem: '<S6>/Use Hardware to Obtain States'
    }

    // End of Update for If: '<S6>/If performing an experiment, grab the PhaseSpace data. Otherwise, use a clock to set time in SIM.' 

    // Update for S-Function (sdspToNetwork): '<S18>/UDP Send'
    sErr = GetErrorBuffer(&Parker_Velocity_Control_DW.UDPSend_NetworkLib[0U]);
    LibUpdate_Network(&Parker_Velocity_Control_DW.UDPSend_NetworkLib[0U],
                      &Parker_Velocity_Control_B.SetUniversalTimeIfthisisasimula,
                      1);
    if (*sErr != 0) {
      rtmSetErrorStatus(Parker_Velocity_Control_M, sErr);
      rtmSetStopRequested(Parker_Velocity_Control_M, 1);
    }

    // End of Update for S-Function (sdspToNetwork): '<S18>/UDP Send'
  }

  // signal main to stop simulation
  {                                    // Sample time: [0.0s, 0.0s]
    if ((rtmGetTFinal(Parker_Velocity_Control_M)!=-1) &&
        !((rtmGetTFinal(Parker_Velocity_Control_M)-
           Parker_Velocity_Control_M->Timing.t[0]) >
          Parker_Velocity_Control_M->Timing.t[0] * (DBL_EPSILON))) {
      rtmSetErrorStatus(Parker_Velocity_Control_M, "Simulation finished");
    }
  }

  // Update absolute time
  // The "clockTick0" counts the number of times the code of this task has
  //  been executed. The absolute time is the multiplication of "clockTick0"
  //  and "Timing.stepSize0". Size of "clockTick0" ensures timer will not
  //  overflow during the application lifespan selected.

  Parker_Velocity_Control_M->Timing.t[0] =
    ((time_T)(++Parker_Velocity_Control_M->Timing.clockTick0)) *
    Parker_Velocity_Control_M->Timing.stepSize0;

  // Update absolute time
  // The "clockTick1" counts the number of times the code of this task has
  //  been executed. The resolution of this integer timer is 0.05, which is the step size
  //  of the task. Size of "clockTick1" ensures timer will not overflow during the
  //  application lifespan selected.

  Parker_Velocity_Control_M->Timing.clockTick1++;
}

// Model step function for TID2
void Parker_Velocity_Control_step2(void) // Sample time: [0.1s, 0.0s]
{
  real_T sampleTime;
  boolean_T rtb_DigitalRead_0;
  real_T sampleTime_0;
  int32_T rtb_WheelDirection;
  real_T rtb_SaturateMotor;

  // MATLABSystem: '<S17>/WhoAmI' incorporates:
  //   DataStoreWrite: '<S17>/Data Store Write'

  if (Parker_Velocity_Control_DW.obj_b.SampleTime !=
      Parker_Velocity_Control_P.WhoAmI_SampleTime) {
    if (((!rtIsInf(Parker_Velocity_Control_P.WhoAmI_SampleTime)) && (!rtIsNaN
          (Parker_Velocity_Control_P.WhoAmI_SampleTime))) || rtIsInf
        (Parker_Velocity_Control_P.WhoAmI_SampleTime)) {
      sampleTime = Parker_Velocity_Control_P.WhoAmI_SampleTime;
    }

    Parker_Velocity_Control_DW.obj_b.SampleTime = sampleTime;
  }

  Parker_Velocity_Control_DW.WhoAmI = whoAmI();

  // End of MATLABSystem: '<S17>/WhoAmI'

  // Outputs for Atomic SubSystem: '<S4>/Send Commands to PWM Blocks'
  // MATLABSystem: '<S33>/RED T1 - BLACK T8' incorporates:
  //   RateTransition: '<S4>/Rate Transition'

  if (Parker_Velocity_Control_DW.RateTransition_Buffer < 1.0) {
    sampleTime = Parker_Velocity_Control_DW.RateTransition_Buffer;
  } else {
    sampleTime = 1.0;
  }

  if (!(sampleTime > 0.0)) {
    sampleTime = 0.0;
  }

  EXT_PWMBlock_setDutyCycle(Parker_Velocity_Control_DW.obj_m.PinNumber,
    sampleTime);

  // End of MATLABSystem: '<S33>/RED T1 - BLACK T8'

  // MATLABSystem: '<S33>/RED T2 - BLACK T3' incorporates:
  //   RateTransition: '<S4>/Rate Transition1'

  if (Parker_Velocity_Control_DW.RateTransition1_Buffer < 1.0) {
    sampleTime = Parker_Velocity_Control_DW.RateTransition1_Buffer;
  } else {
    sampleTime = 1.0;
  }

  if (!(sampleTime > 0.0)) {
    sampleTime = 0.0;
  }

  EXT_PWMBlock_setDutyCycle(Parker_Velocity_Control_DW.obj_ma.PinNumber,
    sampleTime);

  // End of MATLABSystem: '<S33>/RED T2 - BLACK T3'

  // MATLABSystem: '<S33>/RED T3' incorporates:
  //   RateTransition: '<S4>/Rate Transition2'

  if (Parker_Velocity_Control_DW.RateTransition2_Buffer < 1.0) {
    sampleTime = Parker_Velocity_Control_DW.RateTransition2_Buffer;
  } else {
    sampleTime = 1.0;
  }

  if (!(sampleTime > 0.0)) {
    sampleTime = 0.0;
  }

  EXT_PWMBlock_setDutyCycle(Parker_Velocity_Control_DW.obj_h.PinNumber,
    sampleTime);

  // End of MATLABSystem: '<S33>/RED T3'

  // MATLABSystem: '<S33>/RED T4 - BLACK T5' incorporates:
  //   RateTransition: '<S4>/Rate Transition3'

  if (Parker_Velocity_Control_DW.RateTransition3_Buffer < 1.0) {
    sampleTime = Parker_Velocity_Control_DW.RateTransition3_Buffer;
  } else {
    sampleTime = 1.0;
  }

  if (!(sampleTime > 0.0)) {
    sampleTime = 0.0;
  }

  EXT_PWMBlock_setDutyCycle(Parker_Velocity_Control_DW.obj_k.PinNumber,
    sampleTime);

  // End of MATLABSystem: '<S33>/RED T4 - BLACK T5'

  // MATLABSystem: '<S33>/RED T5 - BLACK T4' incorporates:
  //   RateTransition: '<S4>/Rate Transition4'

  if (Parker_Velocity_Control_DW.RateTransition4_Buffer < 1.0) {
    sampleTime = Parker_Velocity_Control_DW.RateTransition4_Buffer;
  } else {
    sampleTime = 1.0;
  }

  if (!(sampleTime > 0.0)) {
    sampleTime = 0.0;
  }

  EXT_PWMBlock_setDutyCycle(Parker_Velocity_Control_DW.obj_hw.PinNumber,
    sampleTime);

  // End of MATLABSystem: '<S33>/RED T5 - BLACK T4'

  // MATLABSystem: '<S33>/RED T6 - BLACK T7' incorporates:
  //   RateTransition: '<S4>/Rate Transition5'

  if (Parker_Velocity_Control_DW.RateTransition5_Buffer < 1.0) {
    sampleTime = Parker_Velocity_Control_DW.RateTransition5_Buffer;
  } else {
    sampleTime = 1.0;
  }

  if (!(sampleTime > 0.0)) {
    sampleTime = 0.0;
  }

  EXT_PWMBlock_setDutyCycle(Parker_Velocity_Control_DW.obj_hf.PinNumber,
    sampleTime);

  // End of MATLABSystem: '<S33>/RED T6 - BLACK T7'

  // MATLABSystem: '<S33>/RED T7 - BLACK T6' incorporates:
  //   RateTransition: '<S4>/Rate Transition6'

  if (Parker_Velocity_Control_DW.RateTransition6_Buffer < 1.0) {
    sampleTime = Parker_Velocity_Control_DW.RateTransition6_Buffer;
  } else {
    sampleTime = 1.0;
  }

  if (!(sampleTime > 0.0)) {
    sampleTime = 0.0;
  }

  EXT_PWMBlock_setDutyCycle(Parker_Velocity_Control_DW.obj_ce.PinNumber,
    sampleTime);

  // End of MATLABSystem: '<S33>/RED T7 - BLACK T6'

  // MATLABSystem: '<S33>/RED T8 - BLACK T1' incorporates:
  //   RateTransition: '<S4>/Rate Transition7'

  if (Parker_Velocity_Control_DW.RateTransition7_Buffer < 1.0) {
    sampleTime = Parker_Velocity_Control_DW.RateTransition7_Buffer;
  } else {
    sampleTime = 1.0;
  }

  if (!(sampleTime > 0.0)) {
    sampleTime = 0.0;
  }

  EXT_PWMBlock_setDutyCycle(Parker_Velocity_Control_DW.obj_fo.PinNumber,
    sampleTime);

  // End of MATLABSystem: '<S33>/RED T8 - BLACK T1'

  // MATLABSystem: '<S33>/BLACK T2' incorporates:
  //   RateTransition: '<S4>/Rate Transition8'

  if (Parker_Velocity_Control_DW.RateTransition8_Buffer < 1.0) {
    sampleTime = Parker_Velocity_Control_DW.RateTransition8_Buffer;
  } else {
    sampleTime = 1.0;
  }

  if (!(sampleTime > 0.0)) {
    sampleTime = 0.0;
  }

  EXT_PWMBlock_setDutyCycle(Parker_Velocity_Control_DW.obj_ci.PinNumber,
    sampleTime);

  // End of MATLABSystem: '<S33>/BLACK T2'
  // End of Outputs for SubSystem: '<S4>/Send Commands to PWM Blocks'

  // If: '<S5>/This IF block determines whether or not to run the RED sim//exp ' incorporates:
  //   Constant: '<S79>/Constant'
  //   DataStoreRead: '<S5>/Data Store Read'

  if ((Parker_Velocity_Control_DW.WhoAmI == 1.0) ||
      (Parker_Velocity_Control_P.simMode == 1.0)) {
    // Outputs for IfAction SubSystem: '<S5>/Change RED Behavior' incorporates:
    //   ActionPort: '<S78>/Action Port'

    // Saturate: '<S80>/Saturate Torque'
    if (Parker_Velocity_Control_DW.RED_Tz_RW >
        Parker_Velocity_Control_P.SaturateTorque_UpperSat) {
      Parker_Velocity_Control_DW.RED_Tz_RW_Sat =
        Parker_Velocity_Control_P.SaturateTorque_UpperSat;
    } else if (Parker_Velocity_Control_DW.RED_Tz_RW <
               Parker_Velocity_Control_P.SaturateTorque_LowerSat) {
      Parker_Velocity_Control_DW.RED_Tz_RW_Sat =
        Parker_Velocity_Control_P.SaturateTorque_LowerSat;
    } else {
      Parker_Velocity_Control_DW.RED_Tz_RW_Sat =
        Parker_Velocity_Control_DW.RED_Tz_RW;
    }

    // End of Saturate: '<S80>/Saturate Torque'

    // Outputs for Atomic SubSystem: '<S81>/Obtain RW Status'
    // MATLABSystem: '<S92>/Digital Read'
    if (Parker_Velocity_Control_DW.obj_f.SampleTime !=
        Parker_Velocity_Control_P.DigitalRead_SampleTime) {
      if (((!rtIsInf(Parker_Velocity_Control_P.DigitalRead_SampleTime)) &&
           (!rtIsNaN(Parker_Velocity_Control_P.DigitalRead_SampleTime))) ||
          rtIsInf(Parker_Velocity_Control_P.DigitalRead_SampleTime)) {
        sampleTime_0 = Parker_Velocity_Control_P.DigitalRead_SampleTime;
      }

      Parker_Velocity_Control_DW.obj_f.SampleTime = sampleTime_0;
    }

    rtb_DigitalRead_0 = MW_gpioRead(24U);

    // End of Outputs for SubSystem: '<S81>/Obtain RW Status'

    // Sum: '<S80>/Sum' incorporates:
    //   DataStoreWrite: '<S78>/RED_Tz_RW_Sat'
    //   Delay: '<S80>/Delay'
    //   MATLAB Function: '<S80>/Calculate RW RPM Increment'

    // MATLAB Function 'From Torque Command RW/Change RED Behavior/Calculate Saturated RPM Signal/Calculate RW RPM Increment': '<S82>:1' 
    // '<S82>:1:3' dRPM_RW =  (Tz_Sat/0.007244720)*(60/2/pi)*0.1;
    sampleTime = Parker_Velocity_Control_DW.RED_Tz_RW_Sat / 0.00724472 *
      9.5492965855137211 * 0.1 + Parker_Velocity_Control_DW.Delay_DSTATE_k;

    // Outputs for Atomic SubSystem: '<S81>/Obtain RW Status'
    // MATLAB Function: '<S81>/Check if RW is Ready' incorporates:
    //   DataStoreWrite: '<S81>/RED_RW_Speed'
    //   Delay: '<S85>/Delay1'
    //   Delay: '<S85>/Delay2'
    //   Delay: '<S85>/Delay3'
    //   Delay: '<S85>/Delay4'
    //   Delay: '<S85>/Delay5'
    //   MATLABSystem: '<S92>/Digital Read'
    //   MinMax: '<S85>/Mostly zeros?'

    // MATLAB Function 'From Torque Command RW/Change RED Behavior/Wheel RPM to PWM/Check if RW is Ready': '<S83>:1' 
    // '<S83>:1:3' if Status == 0
    if ((!rtb_DigitalRead_0) || (!Parker_Velocity_Control_DW.Delay1_DSTATE_kt) ||
        (!Parker_Velocity_Control_DW.Delay2_DSTATE_a[0U]) ||
        (!Parker_Velocity_Control_DW.Delay5_DSTATE_g[0U]) ||
        (!Parker_Velocity_Control_DW.Delay3_DSTATE_n[0U]) ||
        (!Parker_Velocity_Control_DW.Delay4_DSTATE_i[0U])) {
      // '<S83>:1:4' EnableSig    = 1;
      sampleTime_0 = 1.0;

      // '<S83>:1:5' WheelRPM_Out = WheelRPM_In;
      Parker_Velocity_Control_DW.RED_dRz_RW_Sat = sampleTime;
    } else {
      // '<S83>:1:6' else
      // '<S83>:1:7' EnableSig    = 0;
      sampleTime_0 = 0.0;

      // '<S83>:1:8' WheelRPM_Out = 0;
      Parker_Velocity_Control_DW.RED_dRz_RW_Sat = 0.0;
    }

    // End of MATLAB Function: '<S81>/Check if RW is Ready'
    // End of Outputs for SubSystem: '<S81>/Obtain RW Status'

    // Product: '<S84>/Divide' incorporates:
    //   Constant: '<S84>/Gearbox  Ratio'
    //   DataStoreWrite: '<S81>/RED_RW_Speed'

    rtb_SaturateMotor = Parker_Velocity_Control_DW.RED_dRz_RW_Sat *
      Parker_Velocity_Control_P.GearboxRatio_Value;

    // Saturate: '<S81>/Saturate Motor'
    if (rtb_SaturateMotor > Parker_Velocity_Control_P.SaturateMotor_UpperSat) {
      rtb_SaturateMotor = Parker_Velocity_Control_P.SaturateMotor_UpperSat;
    } else {
      if (rtb_SaturateMotor < Parker_Velocity_Control_P.SaturateMotor_LowerSat)
      {
        rtb_SaturateMotor = Parker_Velocity_Control_P.SaturateMotor_LowerSat;
      }
    }

    // End of Saturate: '<S81>/Saturate Motor'

    // MATLAB Function: '<S81>/Get RW Direction'
    // MATLAB Function 'From Torque Command RW/Change RED Behavior/Wheel RPM to PWM/Get RW Direction': '<S86>:1' 
    // '<S86>:1:3' if MotorRPM_Sat > 0
    if (rtb_SaturateMotor > 0.0) {
      // '<S86>:1:4' WheelDirection = 0;
      rtb_WheelDirection = 0;
    } else {
      // '<S86>:1:5' else
      // '<S86>:1:6' WheelDirection = 1;
      rtb_WheelDirection = 1;
    }

    // End of MATLAB Function: '<S81>/Get RW Direction'

    // Outputs for Atomic SubSystem: '<S81>/Send Direction to Motor Controller'
    // MATLABSystem: '<S93>/Digital Write'
    MW_gpioWrite(21U, static_cast<uint8_T>(rtb_WheelDirection));

    // End of Outputs for SubSystem: '<S81>/Send Direction to Motor Controller'

    // MATLAB Function: '<S81>/RPM to PWM Value' incorporates:
    //   Abs: '<S81>/Abs'

    // MATLAB Function 'From Torque Command RW/Change RED Behavior/Wheel RPM to PWM/RPM to PWM Value': '<S88>:1' 
    // '<S88>:1:3' PWMRange = 0.9-0.1;
    // '<S88>:1:5' MotorPWM = ((MotorRPM_Sat*PWMRange)/7000) + 0.1;
    rtb_SaturateMotor = fabs(rtb_SaturateMotor) * 0.8 / 7000.0 + 0.1;

    // Outputs for Atomic SubSystem: '<S81>/Send PWM to Motor Controller'
    // MATLABSystem: '<S90>/PWM1'
    if (!(rtb_SaturateMotor < 1.0)) {
      rtb_SaturateMotor = 1.0;
    }

    EXT_PWMBlock_setDutyCycle(Parker_Velocity_Control_DW.obj_d.PinNumber,
      rtb_SaturateMotor);

    // End of MATLABSystem: '<S90>/PWM1'
    // End of Outputs for SubSystem: '<S81>/Send PWM to Motor Controller'

    // Outputs for Atomic SubSystem: '<S81>/Turn on Motor'
    // MATLABSystem: '<S94>/Digital Write'
    MW_gpioWrite(25U, static_cast<uint8_T>(static_cast<int32_T>(sampleTime_0)));

    // End of Outputs for SubSystem: '<S81>/Turn on Motor'

    // Outputs for Atomic SubSystem: '<S81>/Obtain RW Status'
    // Update for Delay: '<S85>/Delay1' incorporates:
    //   MATLABSystem: '<S92>/Digital Read'

    Parker_Velocity_Control_DW.Delay1_DSTATE_kt = rtb_DigitalRead_0;

    // End of Outputs for SubSystem: '<S81>/Obtain RW Status'

    // Update for Delay: '<S85>/Delay2' incorporates:
    //   MATLABSystem: '<S92>/Digital Read'

    Parker_Velocity_Control_DW.Delay2_DSTATE_a[0] =
      Parker_Velocity_Control_DW.Delay2_DSTATE_a[1];

    // Outputs for Atomic SubSystem: '<S81>/Obtain RW Status'
    Parker_Velocity_Control_DW.Delay2_DSTATE_a[1] = rtb_DigitalRead_0;

    // End of Outputs for SubSystem: '<S81>/Obtain RW Status'

    // Update for Delay: '<S85>/Delay5' incorporates:
    //   MATLABSystem: '<S92>/Digital Read'

    Parker_Velocity_Control_DW.Delay5_DSTATE_g[0] =
      Parker_Velocity_Control_DW.Delay5_DSTATE_g[1];
    Parker_Velocity_Control_DW.Delay5_DSTATE_g[1] =
      Parker_Velocity_Control_DW.Delay5_DSTATE_g[2];

    // Outputs for Atomic SubSystem: '<S81>/Obtain RW Status'
    Parker_Velocity_Control_DW.Delay5_DSTATE_g[2] = rtb_DigitalRead_0;

    // End of Outputs for SubSystem: '<S81>/Obtain RW Status'

    // Update for Delay: '<S85>/Delay3' incorporates:
    //   MATLABSystem: '<S92>/Digital Read'

    Parker_Velocity_Control_DW.Delay3_DSTATE_n[0] =
      Parker_Velocity_Control_DW.Delay3_DSTATE_n[1];
    Parker_Velocity_Control_DW.Delay3_DSTATE_n[1] =
      Parker_Velocity_Control_DW.Delay3_DSTATE_n[2];
    Parker_Velocity_Control_DW.Delay3_DSTATE_n[2] =
      Parker_Velocity_Control_DW.Delay3_DSTATE_n[3];

    // Outputs for Atomic SubSystem: '<S81>/Obtain RW Status'
    Parker_Velocity_Control_DW.Delay3_DSTATE_n[3] = rtb_DigitalRead_0;

    // End of Outputs for SubSystem: '<S81>/Obtain RW Status'

    // Update for Delay: '<S85>/Delay4' incorporates:
    //   MATLABSystem: '<S92>/Digital Read'

    Parker_Velocity_Control_DW.Delay4_DSTATE_i[0] =
      Parker_Velocity_Control_DW.Delay4_DSTATE_i[1];
    Parker_Velocity_Control_DW.Delay4_DSTATE_i[1] =
      Parker_Velocity_Control_DW.Delay4_DSTATE_i[2];
    Parker_Velocity_Control_DW.Delay4_DSTATE_i[2] =
      Parker_Velocity_Control_DW.Delay4_DSTATE_i[3];
    Parker_Velocity_Control_DW.Delay4_DSTATE_i[3] =
      Parker_Velocity_Control_DW.Delay4_DSTATE_i[4];

    // Outputs for Atomic SubSystem: '<S81>/Obtain RW Status'
    Parker_Velocity_Control_DW.Delay4_DSTATE_i[4] = rtb_DigitalRead_0;

    // End of Outputs for SubSystem: '<S81>/Obtain RW Status'

    // Saturate: '<S80>/Saturate RPM'
    if (sampleTime > Parker_Velocity_Control_P.SaturateRPM_UpperSat) {
      // Update for Delay: '<S80>/Delay'
      Parker_Velocity_Control_DW.Delay_DSTATE_k =
        Parker_Velocity_Control_P.SaturateRPM_UpperSat;
    } else if (sampleTime < Parker_Velocity_Control_P.SaturateRPM_LowerSat) {
      // Update for Delay: '<S80>/Delay'
      Parker_Velocity_Control_DW.Delay_DSTATE_k =
        Parker_Velocity_Control_P.SaturateRPM_LowerSat;
    } else {
      // Update for Delay: '<S80>/Delay'
      Parker_Velocity_Control_DW.Delay_DSTATE_k = sampleTime;
    }

    // End of Saturate: '<S80>/Saturate RPM'
    // End of Outputs for SubSystem: '<S5>/Change RED Behavior'
  }

  // End of If: '<S5>/This IF block determines whether or not to run the RED sim//exp ' 
}

// Model step wrapper function for compatibility with a static main program
void Parker_Velocity_Control_step(int_T tid)
{
  switch (tid) {
   case 0 :
    Parker_Velocity_Control_step0();
    break;

   case 2 :
    Parker_Velocity_Control_step2();
    break;

   default :
    break;
  }
}

// Model initialize function
void Parker_Velocity_Control_initialize(void)
{
  // Registration code

  // initialize non-finites
  rt_InitInfAndNaN(sizeof(real_T));

  {
    // Setup solver object
    rtsiSetSimTimeStepPtr(&Parker_Velocity_Control_M->solverInfo,
                          &Parker_Velocity_Control_M->Timing.simTimeStep);
    rtsiSetTPtr(&Parker_Velocity_Control_M->solverInfo, &rtmGetTPtr
                (Parker_Velocity_Control_M));
    rtsiSetStepSizePtr(&Parker_Velocity_Control_M->solverInfo,
                       &Parker_Velocity_Control_M->Timing.stepSize0);
    rtsiSetErrorStatusPtr(&Parker_Velocity_Control_M->solverInfo,
                          (&rtmGetErrorStatus(Parker_Velocity_Control_M)));
    rtsiSetRTModelPtr(&Parker_Velocity_Control_M->solverInfo,
                      Parker_Velocity_Control_M);
  }

  rtsiSetSimTimeStep(&Parker_Velocity_Control_M->solverInfo, MAJOR_TIME_STEP);
  rtsiSetSolverName(&Parker_Velocity_Control_M->solverInfo,"FixedStepDiscrete");
  rtmSetTPtr(Parker_Velocity_Control_M,
             &Parker_Velocity_Control_M->Timing.tArray[0]);
  rtmSetTFinal(Parker_Velocity_Control_M, 690.0);
  Parker_Velocity_Control_M->Timing.stepSize0 = 0.05;

  // Setup for data logging
  {
    static RTWLogInfo rt_DataLoggingInfo;
    rt_DataLoggingInfo.loggingInterval = NULL;
    Parker_Velocity_Control_M->rtwLogInfo = &rt_DataLoggingInfo;
  }

  // Setup for data logging
  {
    rtliSetLogXSignalInfo(Parker_Velocity_Control_M->rtwLogInfo, (NULL));
    rtliSetLogXSignalPtrs(Parker_Velocity_Control_M->rtwLogInfo, (NULL));
    rtliSetLogT(Parker_Velocity_Control_M->rtwLogInfo, "tout");
    rtliSetLogX(Parker_Velocity_Control_M->rtwLogInfo, "");
    rtliSetLogXFinal(Parker_Velocity_Control_M->rtwLogInfo, "");
    rtliSetLogVarNameModifier(Parker_Velocity_Control_M->rtwLogInfo, "rt_");
    rtliSetLogFormat(Parker_Velocity_Control_M->rtwLogInfo, 4);
    rtliSetLogMaxRows(Parker_Velocity_Control_M->rtwLogInfo, 0);
    rtliSetLogDecimation(Parker_Velocity_Control_M->rtwLogInfo, 1);
    rtliSetLogY(Parker_Velocity_Control_M->rtwLogInfo, "");
    rtliSetLogYSignalInfo(Parker_Velocity_Control_M->rtwLogInfo, (NULL));
    rtliSetLogYSignalPtrs(Parker_Velocity_Control_M->rtwLogInfo, (NULL));
  }

  // Matfile logging
  rt_StartDataLoggingWithStartTime(Parker_Velocity_Control_M->rtwLogInfo, 0.0,
    rtmGetTFinal(Parker_Velocity_Control_M),
    Parker_Velocity_Control_M->Timing.stepSize0, (&rtmGetErrorStatus
    (Parker_Velocity_Control_M)));

  {
    char_T *sErr;
    real_T sampleTime;
    uint32_T tseed;
    int32_T t;
    int32_T i;
    real_T tmp;
    static const char_T tmp_0[45] =
      "Unable to configure pin %u for PWM output.\\n";

    // SetupRuntimeResources for ToWorkspace: '<S1>/To Workspace'
    {
      int_T dimensions[1] = { 73 };

      Parker_Velocity_Control_DW.ToWorkspace_PWORK.LoggedData = rt_CreateLogVar(
        Parker_Velocity_Control_M->rtwLogInfo,
        0.0,
        rtmGetTFinal(Parker_Velocity_Control_M),
        Parker_Velocity_Control_M->Timing.stepSize0,
        (&rtmGetErrorStatus(Parker_Velocity_Control_M)),
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
      if (Parker_Velocity_Control_DW.ToWorkspace_PWORK.LoggedData == (NULL))
        return;
    }

    // Start for If: '<S6>/If performing an experiment, grab the PhaseSpace data. Otherwise, use a clock to set time in SIM.' 
    Parker_Velocity_Control_DW.Ifperforminganexperimentgrabthe = -1;

    // Start for S-Function (sdspToNetwork): '<S18>/UDP Send'
    sErr = GetErrorBuffer(&Parker_Velocity_Control_DW.UDPSend_NetworkLib[0U]);
    CreateUDPInterface(&Parker_Velocity_Control_DW.UDPSend_NetworkLib[0U]);
    if (*sErr == 0) {
      LibCreate_Network(&Parker_Velocity_Control_DW.UDPSend_NetworkLib[0U], 1,
                        "0.0.0.0", -1, "192.168.0.105",
                        Parker_Velocity_Control_P.UDPSend_Port, 8192, 8, 0);
    }

    if (*sErr == 0) {
      LibStart(&Parker_Velocity_Control_DW.UDPSend_NetworkLib[0U]);
    }

    if (*sErr != 0) {
      DestroyUDPInterface(&Parker_Velocity_Control_DW.UDPSend_NetworkLib[0U]);
      if (*sErr != 0) {
        rtmSetErrorStatus(Parker_Velocity_Control_M, sErr);
        rtmSetStopRequested(Parker_Velocity_Control_M, 1);
      }
    }

    // End of Start for S-Function (sdspToNetwork): '<S18>/UDP Send'

    // Start for DataStoreMemory: '<Root>/BLACK_Fx'
    Parker_Velocity_Control_DW.BLACK_Fx =
      Parker_Velocity_Control_P.BLACK_Fx_InitialValue;

    // Start for DataStoreMemory: '<Root>/BLACK_Fx1'
    Parker_Velocity_Control_DW.BLUE_Fx =
      Parker_Velocity_Control_P.BLACK_Fx1_InitialValue;

    // Start for DataStoreMemory: '<Root>/BLACK_Fx_Sat'
    Parker_Velocity_Control_DW.BLACK_Fx_Sat =
      Parker_Velocity_Control_P.BLACK_Fx_Sat_InitialValue;

    // Start for DataStoreMemory: '<Root>/BLACK_Fx_Sat1'
    Parker_Velocity_Control_DW.ARM_Elbow_Py =
      Parker_Velocity_Control_P.BLACK_Fx_Sat1_InitialValue;

    // Start for DataStoreMemory: '<Root>/BLACK_Fx_Sat2'
    Parker_Velocity_Control_DW.BLUE_Fx_Sat =
      Parker_Velocity_Control_P.BLACK_Fx_Sat2_InitialValue;

    // Start for DataStoreMemory: '<Root>/BLACK_Fy'
    Parker_Velocity_Control_DW.BLACK_Fy =
      Parker_Velocity_Control_P.BLACK_Fy_InitialValue;

    // Start for DataStoreMemory: '<Root>/BLACK_Fy1'
    Parker_Velocity_Control_DW.BLUE_Fy =
      Parker_Velocity_Control_P.BLACK_Fy1_InitialValue;

    // Start for DataStoreMemory: '<Root>/BLACK_Fy_Sat'
    Parker_Velocity_Control_DW.BLACK_Fy_Sat =
      Parker_Velocity_Control_P.BLACK_Fy_Sat_InitialValue;

    // Start for DataStoreMemory: '<Root>/BLACK_Fy_Sat1'
    Parker_Velocity_Control_DW.ARM_Wrist_Px =
      Parker_Velocity_Control_P.BLACK_Fy_Sat1_InitialValue;

    // Start for DataStoreMemory: '<Root>/BLACK_Fy_Sat2'
    Parker_Velocity_Control_DW.BLUE_Fy_Sat =
      Parker_Velocity_Control_P.BLACK_Fy_Sat2_InitialValue;

    // Start for DataStoreMemory: '<Root>/BLACK_Px'
    Parker_Velocity_Control_DW.BLACK_Px =
      Parker_Velocity_Control_P.BLACK_Px_InitialValue;

    // Start for DataStoreMemory: '<Root>/BLACK_Px1'
    Parker_Velocity_Control_DW.BLUE_Px =
      Parker_Velocity_Control_P.BLACK_Px1_InitialValue;

    // Start for DataStoreMemory: '<Root>/BLACK_Py'
    Parker_Velocity_Control_DW.BLACK_Py =
      Parker_Velocity_Control_P.BLACK_Py_InitialValue;

    // Start for DataStoreMemory: '<Root>/BLACK_Py1'
    Parker_Velocity_Control_DW.BLUE_Py =
      Parker_Velocity_Control_P.BLACK_Py1_InitialValue;

    // Start for DataStoreMemory: '<Root>/BLACK_Rz'
    Parker_Velocity_Control_DW.BLACK_Rz =
      Parker_Velocity_Control_P.BLACK_Rz_InitialValue;

    // Start for DataStoreMemory: '<Root>/BLACK_Rz1'
    Parker_Velocity_Control_DW.BLUE_Rz =
      Parker_Velocity_Control_P.BLACK_Rz1_InitialValue;

    // Start for DataStoreMemory: '<Root>/BLACK_Tz'
    Parker_Velocity_Control_DW.BLACK_Tz =
      Parker_Velocity_Control_P.BLACK_Tz_InitialValue;

    // Start for DataStoreMemory: '<Root>/BLACK_Tz1'
    Parker_Velocity_Control_DW.BLUE_Tz =
      Parker_Velocity_Control_P.BLACK_Tz1_InitialValue;

    // Start for DataStoreMemory: '<Root>/BLACK_Tz_Sat'
    Parker_Velocity_Control_DW.BLACK_Tz_Sat =
      Parker_Velocity_Control_P.BLACK_Tz_Sat_InitialValue;

    // Start for DataStoreMemory: '<Root>/BLACK_Tz_Sat1'
    Parker_Velocity_Control_DW.ARM_Wrist_Py =
      Parker_Velocity_Control_P.BLACK_Tz_Sat1_InitialValue;

    // Start for DataStoreMemory: '<Root>/BLACK_Tz_Sat2'
    Parker_Velocity_Control_DW.BLUE_Tz_Sat =
      Parker_Velocity_Control_P.BLACK_Tz_Sat2_InitialValue;

    // Start for DataStoreMemory: '<Root>/RED_Fx'
    Parker_Velocity_Control_DW.RED_Fx =
      Parker_Velocity_Control_P.RED_Fx_InitialValue;

    // Start for DataStoreMemory: '<Root>/RED_Fx_Sat'
    Parker_Velocity_Control_DW.RED_Fx_Sat =
      Parker_Velocity_Control_P.RED_Fx_Sat_InitialValue;

    // Start for DataStoreMemory: '<Root>/RED_Fy'
    Parker_Velocity_Control_DW.RED_Fy =
      Parker_Velocity_Control_P.RED_Fy_InitialValue;

    // Start for DataStoreMemory: '<Root>/RED_Fy_Sat'
    Parker_Velocity_Control_DW.RED_Fy_Sat =
      Parker_Velocity_Control_P.RED_Fy_Sat_InitialValue;

    // Start for DataStoreMemory: '<Root>/RED_Px'
    Parker_Velocity_Control_DW.RED_Px =
      Parker_Velocity_Control_P.RED_Px_InitialValue;

    // Start for DataStoreMemory: '<Root>/RED_Px1'
    Parker_Velocity_Control_DW.RED_AHRS_Q =
      Parker_Velocity_Control_P.RED_Px1_InitialValue;

    // Start for DataStoreMemory: '<Root>/RED_Px10'
    Parker_Velocity_Control_DW.BLACK_IMU_Ax_b =
      Parker_Velocity_Control_P.RED_Px10_InitialValue;

    // Start for DataStoreMemory: '<Root>/RED_Px11'
    Parker_Velocity_Control_DW.BLACK_IMU_Ay_b =
      Parker_Velocity_Control_P.RED_Px11_InitialValue;

    // Start for DataStoreMemory: '<Root>/RED_Px12'
    Parker_Velocity_Control_DW.BLACK_IMU_Az_b =
      Parker_Velocity_Control_P.RED_Px12_InitialValue;

    // Start for DataStoreMemory: '<Root>/RED_Px19'
    Parker_Velocity_Control_DW.RED_IMU_Q =
      Parker_Velocity_Control_P.RED_Px19_InitialValue;

    // Start for DataStoreMemory: '<Root>/RED_Px2'
    Parker_Velocity_Control_DW.RED_AHRS_P =
      Parker_Velocity_Control_P.RED_Px2_InitialValue;

    // Start for DataStoreMemory: '<Root>/RED_Px20'
    Parker_Velocity_Control_DW.RED_IMU_P =
      Parker_Velocity_Control_P.RED_Px20_InitialValue;

    // Start for DataStoreMemory: '<Root>/RED_Px21'
    Parker_Velocity_Control_DW.RED_IMU_R =
      Parker_Velocity_Control_P.RED_Px21_InitialValue;

    // Start for DataStoreMemory: '<Root>/RED_Px22'
    Parker_Velocity_Control_DW.BLACK_IMU_Q =
      Parker_Velocity_Control_P.RED_Px22_InitialValue;

    // Start for DataStoreMemory: '<Root>/RED_Px23'
    Parker_Velocity_Control_DW.BLACK_IMU_P =
      Parker_Velocity_Control_P.RED_Px23_InitialValue;

    // Start for DataStoreMemory: '<Root>/RED_Px24'
    Parker_Velocity_Control_DW.BLACK_IMU_R =
      Parker_Velocity_Control_P.RED_Px24_InitialValue;

    // Start for DataStoreMemory: '<Root>/RED_Px25'
    Parker_Velocity_Control_DW.RED_IMU_Psi =
      Parker_Velocity_Control_P.RED_Px25_InitialValue;

    // Start for DataStoreMemory: '<Root>/RED_Px26'
    Parker_Velocity_Control_DW.BLACK_IMU_Psi =
      Parker_Velocity_Control_P.RED_Px26_InitialValue;

    // Start for DataStoreMemory: '<Root>/RED_Px3'
    Parker_Velocity_Control_DW.RED_AHRS_R =
      Parker_Velocity_Control_P.RED_Px3_InitialValue;

    // Start for DataStoreMemory: '<Root>/RED_Px31'
    Parker_Velocity_Control_DW.RED_IMU_Ax_I =
      Parker_Velocity_Control_P.RED_Px31_InitialValue;

    // Start for DataStoreMemory: '<Root>/RED_Px32'
    Parker_Velocity_Control_DW.RED_IMU_Ay_I =
      Parker_Velocity_Control_P.RED_Px32_InitialValue;

    // Start for DataStoreMemory: '<Root>/RED_Px37'
    Parker_Velocity_Control_DW.BLACK_IMU_Ax_I =
      Parker_Velocity_Control_P.RED_Px37_InitialValue;

    // Start for DataStoreMemory: '<Root>/RED_Px38'
    Parker_Velocity_Control_DW.BLACK_IMU_Ay_I =
      Parker_Velocity_Control_P.RED_Px38_InitialValue;

    // Start for DataStoreMemory: '<Root>/RED_Px4'
    Parker_Velocity_Control_DW.BLACK_AHRS_Q =
      Parker_Velocity_Control_P.RED_Px4_InitialValue;

    // Start for DataStoreMemory: '<Root>/RED_Px5'
    Parker_Velocity_Control_DW.BLACK_AHRS_P =
      Parker_Velocity_Control_P.RED_Px5_InitialValue;

    // Start for DataStoreMemory: '<Root>/RED_Px6'
    Parker_Velocity_Control_DW.BLACK_AHRS_R =
      Parker_Velocity_Control_P.RED_Px6_InitialValue;

    // Start for DataStoreMemory: '<Root>/RED_Px7'
    Parker_Velocity_Control_DW.RED_IMU_Ax_b =
      Parker_Velocity_Control_P.RED_Px7_InitialValue;

    // Start for DataStoreMemory: '<Root>/RED_Px8'
    Parker_Velocity_Control_DW.RED_IMU_Ay_b =
      Parker_Velocity_Control_P.RED_Px8_InitialValue;

    // Start for DataStoreMemory: '<Root>/RED_Px9'
    Parker_Velocity_Control_DW.RED_IMU_Az_b =
      Parker_Velocity_Control_P.RED_Px9_InitialValue;

    // Start for DataStoreMemory: '<Root>/RED_Py'
    Parker_Velocity_Control_DW.RED_Py =
      Parker_Velocity_Control_P.RED_Py_InitialValue;

    // Start for DataStoreMemory: '<Root>/RED_Rz'
    Parker_Velocity_Control_DW.RED_Rz =
      Parker_Velocity_Control_P.RED_Rz_InitialValue;

    // Start for DataStoreMemory: '<Root>/RED_Tz'
    Parker_Velocity_Control_DW.RED_Tz =
      Parker_Velocity_Control_P.RED_Tz_InitialValue;

    // Start for DataStoreMemory: '<Root>/RED_Tz1'
    Parker_Velocity_Control_DW.RED_Tz_Shoulder =
      Parker_Velocity_Control_P.RED_Tz1_InitialValue;

    // Start for DataStoreMemory: '<Root>/RED_Tz12'
    Parker_Velocity_Control_DW.RED_RzD_Shoulder =
      Parker_Velocity_Control_P.RED_Tz12_InitialValue;

    // Start for DataStoreMemory: '<Root>/RED_Tz13'
    Parker_Velocity_Control_DW.RED_RzD_Elbow =
      Parker_Velocity_Control_P.RED_Tz13_InitialValue;

    // Start for DataStoreMemory: '<Root>/RED_Tz14'
    Parker_Velocity_Control_DW.RED_RzD_Wrist =
      Parker_Velocity_Control_P.RED_Tz14_InitialValue;

    // Start for DataStoreMemory: '<Root>/RED_Tz2'
    Parker_Velocity_Control_DW.RED_Tz_Elbow =
      Parker_Velocity_Control_P.RED_Tz2_InitialValue;

    // Start for DataStoreMemory: '<Root>/RED_Tz3'
    Parker_Velocity_Control_DW.RED_Tz_Wrist =
      Parker_Velocity_Control_P.RED_Tz3_InitialValue;

    // Start for DataStoreMemory: '<Root>/RED_Tz4'
    Parker_Velocity_Control_DW.RED_Rz_Shoulder =
      Parker_Velocity_Control_P.RED_Tz4_InitialValue;

    // Start for DataStoreMemory: '<Root>/RED_Tz5'
    Parker_Velocity_Control_DW.RED_Rz_Elbow =
      Parker_Velocity_Control_P.RED_Tz5_InitialValue;

    // Start for DataStoreMemory: '<Root>/RED_Tz6'
    Parker_Velocity_Control_DW.RED_Rz_Wrist =
      Parker_Velocity_Control_P.RED_Tz6_InitialValue;

    // Start for DataStoreMemory: '<Root>/RED_Tz7'
    Parker_Velocity_Control_DW.Float_State =
      Parker_Velocity_Control_P.RED_Tz7_InitialValue;

    // Start for DataStoreMemory: '<Root>/RED_Tz8'
    Parker_Velocity_Control_DW.Magnet_State =
      Parker_Velocity_Control_P.RED_Tz8_InitialValue;

    // Start for DataStoreMemory: '<Root>/RED_Tz_RW'
    Parker_Velocity_Control_DW.RED_Tz_RW =
      Parker_Velocity_Control_P.RED_Tz_RW_InitialValue;

    // Start for DataStoreMemory: '<Root>/RED_Tz_RW Sat'
    Parker_Velocity_Control_DW.RED_Tz_RW_Sat =
      Parker_Velocity_Control_P.RED_Tz_RWSat_InitialValue;

    // Start for DataStoreMemory: '<Root>/RED_Tz_RW Sat1'
    Parker_Velocity_Control_DW.ARM_EndEff_Py =
      Parker_Velocity_Control_P.RED_Tz_RWSat1_InitialValue;

    // Start for DataStoreMemory: '<Root>/RED_Tz_RW1'
    Parker_Velocity_Control_DW.ARM_EndEff_Px =
      Parker_Velocity_Control_P.RED_Tz_RW1_InitialValue;

    // Start for DataStoreMemory: '<Root>/RED_Tz_Sat'
    Parker_Velocity_Control_DW.RED_Tz_Sat =
      Parker_Velocity_Control_P.RED_Tz_Sat_InitialValue;

    // Start for DataStoreMemory: '<Root>/RED_Tz_Sat1'
    Parker_Velocity_Control_DW.ARM_Elbow_Px =
      Parker_Velocity_Control_P.RED_Tz_Sat1_InitialValue;

    // Start for DataStoreMemory: '<Root>/RED_dRz_RW Sat'
    Parker_Velocity_Control_DW.RED_dRz_RW_Sat =
      Parker_Velocity_Control_P.RED_dRz_RWSat_InitialValue;

    // Start for DataStoreMemory: '<Root>/Universal_Time'
    Parker_Velocity_Control_DW.Univ_Time =
      Parker_Velocity_Control_P.Universal_Time_InitialValue;

    // Start for DataStoreMemory: '<Root>/Universal_Time1'
    Parker_Velocity_Control_DW.RED_Vx =
      Parker_Velocity_Control_P.Universal_Time1_InitialValue;

    // Start for DataStoreMemory: '<Root>/Universal_Time10'
    Parker_Velocity_Control_DW.RED_Ax =
      Parker_Velocity_Control_P.Universal_Time10_InitialValue;

    // Start for DataStoreMemory: '<Root>/Universal_Time11'
    Parker_Velocity_Control_DW.RED_Ay =
      Parker_Velocity_Control_P.Universal_Time11_InitialValue;

    // Start for DataStoreMemory: '<Root>/Universal_Time12'
    Parker_Velocity_Control_DW.BLACK_Ax =
      Parker_Velocity_Control_P.Universal_Time12_InitialValue;

    // Start for DataStoreMemory: '<Root>/Universal_Time13'
    Parker_Velocity_Control_DW.BLACK_Ay =
      Parker_Velocity_Control_P.Universal_Time13_InitialValue;

    // Start for DataStoreMemory: '<Root>/Universal_Time14'
    Parker_Velocity_Control_DW.BLACK_RzDD =
      Parker_Velocity_Control_P.Universal_Time14_InitialValue;

    // Start for DataStoreMemory: '<Root>/Universal_Time15'
    Parker_Velocity_Control_DW.RED_RzDD =
      Parker_Velocity_Control_P.Universal_Time15_InitialValue;

    // Start for DataStoreMemory: '<Root>/Universal_Time19'
    Parker_Velocity_Control_DW.WhoAmI =
      Parker_Velocity_Control_P.Universal_Time19_InitialValue;

    // Start for DataStoreMemory: '<Root>/Universal_Time2'
    Parker_Velocity_Control_DW.RED_Vy =
      Parker_Velocity_Control_P.Universal_Time2_InitialValue;

    // Start for DataStoreMemory: '<Root>/Universal_Time3'
    Parker_Velocity_Control_DW.RED_RzD =
      Parker_Velocity_Control_P.Universal_Time3_InitialValue;

    // Start for DataStoreMemory: '<Root>/Universal_Time4'
    Parker_Velocity_Control_DW.BLACK_Vx =
      Parker_Velocity_Control_P.Universal_Time4_InitialValue;

    // Start for DataStoreMemory: '<Root>/Universal_Time5'
    Parker_Velocity_Control_DW.BLACK_Vy =
      Parker_Velocity_Control_P.Universal_Time5_InitialValue;

    // Start for DataStoreMemory: '<Root>/Universal_Time6'
    Parker_Velocity_Control_DW.BLACK_RzD =
      Parker_Velocity_Control_P.Universal_Time6_InitialValue;

    // Start for DataStoreMemory: '<Root>/Universal_Time7'
    Parker_Velocity_Control_DW.BLUE_Vx =
      Parker_Velocity_Control_P.Universal_Time7_InitialValue;

    // Start for DataStoreMemory: '<Root>/Universal_Time8'
    Parker_Velocity_Control_DW.BLUE_Vy =
      Parker_Velocity_Control_P.Universal_Time8_InitialValue;

    // Start for DataStoreMemory: '<Root>/Universal_Time9'
    Parker_Velocity_Control_DW.BLUE_RzD =
      Parker_Velocity_Control_P.Universal_Time9_InitialValue;

    // SystemInitialize for IfAction SubSystem: '<Root>/Phase #2:  Move to  Initial Position' 
    // SystemInitialize for IfAction SubSystem: '<S12>/Change BLACK Behavior'
    // InitializeConditions for Delay: '<S158>/Delay1'
    Parker_Velocity_Control_DW.icLoad_ns = 1U;

    // InitializeConditions for Delay: '<S160>/Delay1'
    Parker_Velocity_Control_DW.icLoad_a = 1U;

    // InitializeConditions for Delay: '<S156>/Delay1'
    Parker_Velocity_Control_DW.icLoad_h = 1U;

    // SystemInitialize for IfAction SubSystem: '<S158>/Hold this value'
    // SystemInitialize for Outport: '<S159>/Out1'
    Parker_Velocity_Control_B.In1_ej = Parker_Velocity_Control_P.Out1_Y0_av;

    // End of SystemInitialize for SubSystem: '<S158>/Hold this value'

    // SystemInitialize for IfAction SubSystem: '<S160>/Hold this value'
    // SystemInitialize for Outport: '<S161>/Out1'
    Parker_Velocity_Control_B.In1_g = Parker_Velocity_Control_P.Out1_Y0_h4;

    // End of SystemInitialize for SubSystem: '<S160>/Hold this value'

    // SystemInitialize for IfAction SubSystem: '<S156>/Hold this value'
    // SystemInitialize for Outport: '<S157>/Out1'
    Parker_Velocity_Control_B.In1_d = Parker_Velocity_Control_P.Out1_Y0_aw;

    // End of SystemInitialize for SubSystem: '<S156>/Hold this value'
    // End of SystemInitialize for SubSystem: '<S12>/Change BLACK Behavior'

    // SystemInitialize for IfAction SubSystem: '<S12>/Change BLUE Behavior'
    // InitializeConditions for Delay: '<S171>/Delay1'
    Parker_Velocity_Control_DW.icLoad_dy = 1U;

    // InitializeConditions for Delay: '<S173>/Delay1'
    Parker_Velocity_Control_DW.icLoad_n = 1U;

    // InitializeConditions for Delay: '<S169>/Delay1'
    Parker_Velocity_Control_DW.icLoad_bp = 1U;

    // SystemInitialize for IfAction SubSystem: '<S171>/Hold this value'
    // SystemInitialize for Outport: '<S172>/Out1'
    Parker_Velocity_Control_B.In1_l = Parker_Velocity_Control_P.Out1_Y0_a4;

    // End of SystemInitialize for SubSystem: '<S171>/Hold this value'

    // SystemInitialize for IfAction SubSystem: '<S173>/Hold this value'
    // SystemInitialize for Outport: '<S174>/Out1'
    Parker_Velocity_Control_B.In1_f = Parker_Velocity_Control_P.Out1_Y0_kw;

    // End of SystemInitialize for SubSystem: '<S173>/Hold this value'

    // SystemInitialize for IfAction SubSystem: '<S169>/Hold this value'
    // SystemInitialize for Outport: '<S170>/Out1'
    Parker_Velocity_Control_B.In1_fj = Parker_Velocity_Control_P.Out1_Y0_mf;

    // End of SystemInitialize for SubSystem: '<S169>/Hold this value'
    // End of SystemInitialize for SubSystem: '<S12>/Change BLUE Behavior'
    // End of SystemInitialize for SubSystem: '<Root>/Phase #2:  Move to  Initial Position' 

    // SystemInitialize for IfAction SubSystem: '<Root>/Phase #3: Experiment'
    // SystemInitialize for IfAction SubSystem: '<S13>/Change BLACK Behavior'
    // SystemInitialize for IfAction SubSystem: '<S180>/Sub-Phase #1'
    Parker_Veloc_SubPhase1_Init(&Parker_Velocity_Control_B.SubPhase1,
      &Parker_Velocity_Control_DW.SubPhase1,
      &Parker_Velocity_Control_P.SubPhase1);

    // End of SystemInitialize for SubSystem: '<S180>/Sub-Phase #1'

    // SystemInitialize for IfAction SubSystem: '<S180>/Sub-Phase #2 '
    Parker_Veloc_SubPhase1_Init(&Parker_Velocity_Control_B.SubPhase2,
      &Parker_Velocity_Control_DW.SubPhase2,
      &Parker_Velocity_Control_P.SubPhase2);

    // End of SystemInitialize for SubSystem: '<S180>/Sub-Phase #2 '

    // SystemInitialize for IfAction SubSystem: '<S180>/Sub-Phase #3 '
    Parker_Veloc_SubPhase1_Init(&Parker_Velocity_Control_B.SubPhase3,
      &Parker_Velocity_Control_DW.SubPhase3,
      &Parker_Velocity_Control_P.SubPhase3);

    // End of SystemInitialize for SubSystem: '<S180>/Sub-Phase #3 '

    // SystemInitialize for IfAction SubSystem: '<S180>/Sub-Phase #4'
    Parker_Veloc_SubPhase1_Init(&Parker_Velocity_Control_B.SubPhase4,
      &Parker_Velocity_Control_DW.SubPhase4,
      &Parker_Velocity_Control_P.SubPhase4);

    // End of SystemInitialize for SubSystem: '<S180>/Sub-Phase #4'
    // End of SystemInitialize for SubSystem: '<S13>/Change BLACK Behavior'

    // SystemInitialize for IfAction SubSystem: '<S13>/Change BLUE Behavior'
    // SystemInitialize for IfAction SubSystem: '<S181>/Sub-Phase #1'
    Parker_Vel_SubPhase1_n_Init(&Parker_Velocity_Control_B.SubPhase1_a,
      &Parker_Velocity_Control_DW.SubPhase1_a,
      &Parker_Velocity_Control_P.SubPhase1_a);

    // End of SystemInitialize for SubSystem: '<S181>/Sub-Phase #1'

    // SystemInitialize for IfAction SubSystem: '<S181>/Sub-Phase #2 '
    Parker_Vel_SubPhase1_n_Init(&Parker_Velocity_Control_B.SubPhase2_m,
      &Parker_Velocity_Control_DW.SubPhase2_m,
      &Parker_Velocity_Control_P.SubPhase2_m);

    // End of SystemInitialize for SubSystem: '<S181>/Sub-Phase #2 '

    // SystemInitialize for IfAction SubSystem: '<S181>/Sub-Phase #3 '
    Parker_Vel_SubPhase1_n_Init(&Parker_Velocity_Control_B.SubPhase3_p,
      &Parker_Velocity_Control_DW.SubPhase3_p,
      &Parker_Velocity_Control_P.SubPhase3_p);

    // End of SystemInitialize for SubSystem: '<S181>/Sub-Phase #3 '

    // SystemInitialize for IfAction SubSystem: '<S181>/Sub-Phase #4'
    Parker_Vel_SubPhase1_n_Init(&Parker_Velocity_Control_B.SubPhase4_f,
      &Parker_Velocity_Control_DW.SubPhase4_f,
      &Parker_Velocity_Control_P.SubPhase4_f);

    // End of SystemInitialize for SubSystem: '<S181>/Sub-Phase #4'
    // End of SystemInitialize for SubSystem: '<S13>/Change BLUE Behavior'

    // SystemInitialize for IfAction SubSystem: '<S13>/Change RED Behavior'
    // SystemInitialize for IfAction SubSystem: '<S182>/Sub-Phase #4'
    // SystemInitialize for IfAction SubSystem: '<S337>/RED+ARM'
    // Start for MATLABSystem: '<S346>/Send Speed Commands to  Manipulator'
    Parker_Velocity_Control_DW.obj_o.matlabCodegenIsDeleted = true;
    Parker_Velocity_Control_DW.obj_o.isInitialized = 0;
    Parker_Velocity_Control_DW.obj_o.matlabCodegenIsDeleted = false;
    Parker_Velocity_Control_DW.obj_o.isSetupComplete = false;
    Parker_Velocity_Control_DW.obj_o.isInitialized = 1;
    Parker_Velocity_Control_DW.obj_o.isSetupComplete = true;

    // End of SystemInitialize for SubSystem: '<S337>/RED+ARM'
    // End of SystemInitialize for SubSystem: '<S182>/Sub-Phase #4'
    // End of SystemInitialize for SubSystem: '<S13>/Change RED Behavior'
    // End of SystemInitialize for SubSystem: '<Root>/Phase #3: Experiment'

    // SystemInitialize for IfAction SubSystem: '<Root>/Phase #4:  Return Home'
    // SystemInitialize for IfAction SubSystem: '<S14>/Change BLACK Behavior'
    // InitializeConditions for Delay: '<S357>/Delay1'
    Parker_Velocity_Control_DW.icLoad_e = 1U;

    // InitializeConditions for Delay: '<S359>/Delay1'
    Parker_Velocity_Control_DW.icLoad_fr = 1U;

    // InitializeConditions for Delay: '<S355>/Delay1'
    Parker_Velocity_Control_DW.icLoad_o = 1U;

    // SystemInitialize for IfAction SubSystem: '<S357>/Hold this value'
    // SystemInitialize for Outport: '<S358>/Out1'
    Parker_Velocity_Control_B.In1_b = Parker_Velocity_Control_P.Out1_Y0_ii;

    // End of SystemInitialize for SubSystem: '<S357>/Hold this value'

    // SystemInitialize for IfAction SubSystem: '<S359>/Hold this value'
    // SystemInitialize for Outport: '<S360>/Out1'
    Parker_Velocity_Control_B.In1_o5 = Parker_Velocity_Control_P.Out1_Y0_hq;

    // End of SystemInitialize for SubSystem: '<S359>/Hold this value'

    // SystemInitialize for IfAction SubSystem: '<S355>/Hold this value'
    // SystemInitialize for Outport: '<S356>/Out1'
    Parker_Velocity_Control_B.In1_ei = Parker_Velocity_Control_P.Out1_Y0_o;

    // End of SystemInitialize for SubSystem: '<S355>/Hold this value'
    // End of SystemInitialize for SubSystem: '<S14>/Change BLACK Behavior'

    // SystemInitialize for IfAction SubSystem: '<S14>/Change BLUE Behavior'
    P_ChangeBLUEBehavior_a_Init(&Parker_Velocity_Control_B.ChangeBLUEBehavior_gj,
      &Parker_Velocity_Control_DW.ChangeBLUEBehavior_gj,
      &Parker_Velocity_Control_P.ChangeBLUEBehavior_gj);

    // End of SystemInitialize for SubSystem: '<S14>/Change BLUE Behavior'

    // SystemInitialize for IfAction SubSystem: '<S14>/Change RED Behavior'
    // InitializeConditions for Delay: '<S383>/Delay1'
    Parker_Velocity_Control_DW.icLoad_k = 1U;

    // InitializeConditions for Delay: '<S385>/Delay1'
    Parker_Velocity_Control_DW.icLoad_du = 1U;

    // InitializeConditions for Delay: '<S387>/Delay1'
    Parker_Velocity_Control_DW.icLoad_g = 1U;

    // SystemInitialize for IfAction SubSystem: '<S383>/Hold this value'
    // SystemInitialize for Outport: '<S384>/Out1'
    Parker_Velocity_Control_B.In1_o = Parker_Velocity_Control_P.Out1_Y0_lc;

    // End of SystemInitialize for SubSystem: '<S383>/Hold this value'

    // SystemInitialize for IfAction SubSystem: '<S385>/Hold this value'
    // SystemInitialize for Outport: '<S386>/Out1'
    Parker_Velocity_Control_B.In1_c = Parker_Velocity_Control_P.Out1_Y0_of;

    // End of SystemInitialize for SubSystem: '<S385>/Hold this value'

    // SystemInitialize for IfAction SubSystem: '<S387>/Hold this value'
    // SystemInitialize for Outport: '<S388>/Out1'
    Parker_Velocity_Control_B.In1_a = Parker_Velocity_Control_P.Out1_Y0_l2;

    // End of SystemInitialize for SubSystem: '<S387>/Hold this value'
    // End of SystemInitialize for SubSystem: '<S14>/Change RED Behavior'
    // End of SystemInitialize for SubSystem: '<Root>/Phase #4:  Return Home'

    // SystemInitialize for IfAction SubSystem: '<Root>/Phase #5:  Hold Home'
    // SystemInitialize for IfAction SubSystem: '<S15>/Change BLACK Behavior'
    // InitializeConditions for Delay: '<S410>/Delay1'
    Parker_Velocity_Control_DW.icLoad_d = 1U;

    // InitializeConditions for Delay: '<S412>/Delay1'
    Parker_Velocity_Control_DW.icLoad_f = 1U;

    // InitializeConditions for Delay: '<S408>/Delay1'
    Parker_Velocity_Control_DW.icLoad_b = 1U;

    // SystemInitialize for IfAction SubSystem: '<S410>/Hold this value'
    // SystemInitialize for Outport: '<S411>/Out1'
    Parker_Velocity_Control_B.In1_it = Parker_Velocity_Control_P.Out1_Y0_c2;

    // End of SystemInitialize for SubSystem: '<S410>/Hold this value'

    // SystemInitialize for IfAction SubSystem: '<S412>/Hold this value'
    // SystemInitialize for Outport: '<S413>/Out1'
    Parker_Velocity_Control_B.In1_p = Parker_Velocity_Control_P.Out1_Y0_f2b;

    // End of SystemInitialize for SubSystem: '<S412>/Hold this value'

    // SystemInitialize for IfAction SubSystem: '<S408>/Hold this value'
    // SystemInitialize for Outport: '<S409>/Out1'
    Parker_Velocity_Control_B.In1_j = Parker_Velocity_Control_P.Out1_Y0_by;

    // End of SystemInitialize for SubSystem: '<S408>/Hold this value'
    // End of SystemInitialize for SubSystem: '<S15>/Change BLACK Behavior'

    // SystemInitialize for IfAction SubSystem: '<S15>/Change BLUE Behavior'
    P_ChangeBLUEBehavior_a_Init(&Parker_Velocity_Control_B.ChangeBLUEBehavior_b,
      &Parker_Velocity_Control_DW.ChangeBLUEBehavior_b,
      &Parker_Velocity_Control_P.ChangeBLUEBehavior_b);

    // End of SystemInitialize for SubSystem: '<S15>/Change BLUE Behavior'

    // SystemInitialize for IfAction SubSystem: '<S15>/Change RED Behavior'
    // InitializeConditions for Delay: '<S436>/Delay1'
    Parker_Velocity_Control_DW.icLoad = 1U;

    // InitializeConditions for Delay: '<S438>/Delay1'
    Parker_Velocity_Control_DW.icLoad_l = 1U;

    // InitializeConditions for Delay: '<S440>/Delay1'
    Parker_Velocity_Control_DW.icLoad_j = 1U;

    // SystemInitialize for IfAction SubSystem: '<S436>/Hold this value'
    // SystemInitialize for Outport: '<S437>/Out1'
    Parker_Velocity_Control_B.In1_i = Parker_Velocity_Control_P.Out1_Y0_jf;

    // End of SystemInitialize for SubSystem: '<S436>/Hold this value'

    // SystemInitialize for IfAction SubSystem: '<S438>/Hold this value'
    // SystemInitialize for Outport: '<S439>/Out1'
    Parker_Velocity_Control_B.In1_e = Parker_Velocity_Control_P.Out1_Y0_fn;

    // End of SystemInitialize for SubSystem: '<S438>/Hold this value'

    // SystemInitialize for IfAction SubSystem: '<S440>/Hold this value'
    // SystemInitialize for Outport: '<S441>/Out1'
    Parker_Velocity_Control_B.In1 = Parker_Velocity_Control_P.Out1_Y0_a2;

    // End of SystemInitialize for SubSystem: '<S440>/Hold this value'
    // End of SystemInitialize for SubSystem: '<S15>/Change RED Behavior'
    // End of SystemInitialize for SubSystem: '<Root>/Phase #5:  Hold Home'

    // SystemInitialize for IfAction SubSystem: '<Root>/Simulate Plant Dynamics' 
    // InitializeConditions for DiscreteIntegrator: '<S461>/Acceleration  to Velocity' 
    Parker_Velocity_Control_DW.AccelerationtoVelocity_DSTATE[0] =
      Parker_Velocity_Control_P.AccelerationtoVelocity_IC_i;

    // InitializeConditions for DiscreteIntegrator: '<S461>/Velocity to Position' 
    Parker_Velocity_Control_DW.VelocitytoPosition_DSTATE[0] =
      Parker_Velocity_Control_P.drop_states_BLACK[0];

    // InitializeConditions for DiscreteIntegrator: '<S461>/Acceleration  to Velocity' 
    Parker_Velocity_Control_DW.AccelerationtoVelocity_DSTATE[1] =
      Parker_Velocity_Control_P.AccelerationtoVelocity_IC_i;

    // InitializeConditions for DiscreteIntegrator: '<S461>/Velocity to Position' 
    Parker_Velocity_Control_DW.VelocitytoPosition_DSTATE[1] =
      Parker_Velocity_Control_P.drop_states_BLACK[1];

    // InitializeConditions for DiscreteIntegrator: '<S461>/Acceleration  to Velocity' 
    Parker_Velocity_Control_DW.AccelerationtoVelocity_DSTATE[2] =
      Parker_Velocity_Control_P.AccelerationtoVelocity_IC_i;

    // InitializeConditions for DiscreteIntegrator: '<S461>/Velocity to Position' 
    Parker_Velocity_Control_DW.VelocitytoPosition_DSTATE[2] =
      Parker_Velocity_Control_P.drop_states_BLACK[2];

    // InitializeConditions for RandomNumber: '<S461>/Random Number'
    tmp = floor(Parker_Velocity_Control_P.RandomNumber_Seed_h);
    if (rtIsNaN(tmp) || rtIsInf(tmp)) {
      tmp = 0.0;
    } else {
      tmp = fmod(tmp, 4.294967296E+9);
    }

    tseed = tmp < 0.0 ? static_cast<uint32_T>(-static_cast<int32_T>
      (static_cast<uint32_T>(-tmp))) : static_cast<uint32_T>(tmp);
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

    Parker_Velocity_Control_DW.RandSeed = tseed;
    Parker_Velocity_Control_DW.NextOutput = Park_rt_nrand_Upu32_Yd_f_pw_snf
      (&Parker_Velocity_Control_DW.RandSeed) * sqrt
      (Parker_Velocity_Control_P.noise_variance_BLACK) +
      Parker_Velocity_Control_P.RandomNumber_Mean_n;

    // End of InitializeConditions for RandomNumber: '<S461>/Random Number'

    // InitializeConditions for Delay: '<S466>/Delay1'
    Parker_Velocity_Control_DW.icLoad_bb = 1U;

    // InitializeConditions for Delay: '<S464>/Delay1'
    Parker_Velocity_Control_DW.icLoad_p = 1U;

    // InitializeConditions for RandomNumber: '<S457>/Random Number1'
    tmp = floor(Parker_Velocity_Control_P.RandomNumber1_Seed_m);
    if (rtIsNaN(tmp) || rtIsInf(tmp)) {
      tmp = 0.0;
    } else {
      tmp = fmod(tmp, 4.294967296E+9);
    }

    tseed = tmp < 0.0 ? static_cast<uint32_T>(-static_cast<int32_T>(static_cast<
      uint32_T>(-tmp))) : static_cast<uint32_T>(tmp);
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

    Parker_Velocity_Control_DW.RandSeed_m = tseed;
    Parker_Velocity_Control_DW.NextOutput_p = Park_rt_nrand_Upu32_Yd_f_pw_snf
      (&Parker_Velocity_Control_DW.RandSeed_m) *
      Parker_Velocity_Control_P.RandomNumber1_StdDev_o +
      Parker_Velocity_Control_P.RandomNumber1_Mean_i;

    // End of InitializeConditions for RandomNumber: '<S457>/Random Number1'

    // InitializeConditions for Delay: '<S462>/Delay1'
    Parker_Velocity_Control_DW.icLoad_lv = 1U;

    // InitializeConditions for Delay: '<S465>/Delay1'
    Parker_Velocity_Control_DW.icLoad_hu = 1U;

    // InitializeConditions for RandomNumber: '<S457>/Random Number'
    tmp = floor(Parker_Velocity_Control_P.RandomNumber_Seed_a);
    if (rtIsNaN(tmp) || rtIsInf(tmp)) {
      tmp = 0.0;
    } else {
      tmp = fmod(tmp, 4.294967296E+9);
    }

    tseed = tmp < 0.0 ? static_cast<uint32_T>(-static_cast<int32_T>(static_cast<
      uint32_T>(-tmp))) : static_cast<uint32_T>(tmp);
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

    Parker_Velocity_Control_DW.RandSeed_d = tseed;
    Parker_Velocity_Control_DW.NextOutput_o = Park_rt_nrand_Upu32_Yd_f_pw_snf
      (&Parker_Velocity_Control_DW.RandSeed_d) *
      Parker_Velocity_Control_P.RandomNumber_StdDev +
      Parker_Velocity_Control_P.RandomNumber_Mean_e;

    // End of InitializeConditions for RandomNumber: '<S457>/Random Number'

    // InitializeConditions for Delay: '<S463>/Delay1'
    Parker_Velocity_Control_DW.icLoad_m = 1U;

    // InitializeConditions for RandomNumber: '<S457>/Random Number2'
    tmp = floor(Parker_Velocity_Control_P.RandomNumber2_Seed_i);
    if (rtIsNaN(tmp) || rtIsInf(tmp)) {
      tmp = 0.0;
    } else {
      tmp = fmod(tmp, 4.294967296E+9);
    }

    tseed = tmp < 0.0 ? static_cast<uint32_T>(-static_cast<int32_T>(static_cast<
      uint32_T>(-tmp))) : static_cast<uint32_T>(tmp);
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

    Parker_Velocity_Control_DW.RandSeed_k = tseed;
    Parker_Velocity_Control_DW.NextOutput_j = Park_rt_nrand_Upu32_Yd_f_pw_snf
      (&Parker_Velocity_Control_DW.RandSeed_k) *
      Parker_Velocity_Control_P.RandomNumber2_StdDev_p +
      Parker_Velocity_Control_P.RandomNumber2_Mean_c;

    // End of InitializeConditions for RandomNumber: '<S457>/Random Number2'

    // InitializeConditions for DiscreteIntegrator: '<S473>/Acceleration  to Velocity' 
    Parker_Velocity_Control_DW.AccelerationtoVelocity_DSTATE_b[0] =
      Parker_Velocity_Control_P.AccelerationtoVelocity_IC_p;

    // InitializeConditions for DiscreteIntegrator: '<S473>/Velocity to Position' 
    Parker_Velocity_Control_DW.VelocitytoPosition_DSTATE_a[0] =
      Parker_Velocity_Control_P.drop_states_BLUE[0];

    // InitializeConditions for DiscreteIntegrator: '<S473>/Acceleration  to Velocity' 
    Parker_Velocity_Control_DW.AccelerationtoVelocity_DSTATE_b[1] =
      Parker_Velocity_Control_P.AccelerationtoVelocity_IC_p;

    // InitializeConditions for DiscreteIntegrator: '<S473>/Velocity to Position' 
    Parker_Velocity_Control_DW.VelocitytoPosition_DSTATE_a[1] =
      Parker_Velocity_Control_P.drop_states_BLUE[1];

    // InitializeConditions for DiscreteIntegrator: '<S473>/Acceleration  to Velocity' 
    Parker_Velocity_Control_DW.AccelerationtoVelocity_DSTATE_b[2] =
      Parker_Velocity_Control_P.AccelerationtoVelocity_IC_p;

    // InitializeConditions for DiscreteIntegrator: '<S473>/Velocity to Position' 
    Parker_Velocity_Control_DW.VelocitytoPosition_DSTATE_a[2] =
      Parker_Velocity_Control_P.drop_states_BLUE[2];

    // InitializeConditions for RandomNumber: '<S473>/Random Number'
    tmp = floor(Parker_Velocity_Control_P.RandomNumber_Seed_k);
    if (rtIsNaN(tmp) || rtIsInf(tmp)) {
      tmp = 0.0;
    } else {
      tmp = fmod(tmp, 4.294967296E+9);
    }

    tseed = tmp < 0.0 ? static_cast<uint32_T>(-static_cast<int32_T>(static_cast<
      uint32_T>(-tmp))) : static_cast<uint32_T>(tmp);
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

    Parker_Velocity_Control_DW.RandSeed_d3 = tseed;
    Parker_Velocity_Control_DW.NextOutput_g = Park_rt_nrand_Upu32_Yd_f_pw_snf
      (&Parker_Velocity_Control_DW.RandSeed_d3) * sqrt
      (Parker_Velocity_Control_P.noise_variance_BLUE) +
      Parker_Velocity_Control_P.RandomNumber_Mean_b;

    // End of InitializeConditions for RandomNumber: '<S473>/Random Number'

    // InitializeConditions for Delay: '<S478>/Delay1'
    Parker_Velocity_Control_DW.icLoad_dc = 1U;

    // InitializeConditions for Delay: '<S476>/Delay1'
    Parker_Velocity_Control_DW.icLoad_ln = 1U;

    // InitializeConditions for RandomNumber: '<S458>/Random Number4'
    tmp = floor(Parker_Velocity_Control_P.RandomNumber4_Seed);
    if (rtIsNaN(tmp) || rtIsInf(tmp)) {
      tmp = 0.0;
    } else {
      tmp = fmod(tmp, 4.294967296E+9);
    }

    tseed = tmp < 0.0 ? static_cast<uint32_T>(-static_cast<int32_T>(static_cast<
      uint32_T>(-tmp))) : static_cast<uint32_T>(tmp);
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

    Parker_Velocity_Control_DW.RandSeed_dc = tseed;
    Park_rt_nrand_Upu32_Yd_f_pw_snf(&Parker_Velocity_Control_DW.RandSeed_dc);

    // End of InitializeConditions for RandomNumber: '<S458>/Random Number4'

    // InitializeConditions for Delay: '<S474>/Delay1'
    Parker_Velocity_Control_DW.icLoad_oc = 1U;

    // InitializeConditions for Delay: '<S477>/Delay1'
    Parker_Velocity_Control_DW.icLoad_l1 = 1U;

    // InitializeConditions for RandomNumber: '<S458>/Random Number3'
    tmp = floor(Parker_Velocity_Control_P.RandomNumber3_Seed_e);
    if (rtIsNaN(tmp) || rtIsInf(tmp)) {
      tmp = 0.0;
    } else {
      tmp = fmod(tmp, 4.294967296E+9);
    }

    tseed = tmp < 0.0 ? static_cast<uint32_T>(-static_cast<int32_T>(static_cast<
      uint32_T>(-tmp))) : static_cast<uint32_T>(tmp);
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

    Parker_Velocity_Control_DW.RandSeed_j = tseed;
    Park_rt_nrand_Upu32_Yd_f_pw_snf(&Parker_Velocity_Control_DW.RandSeed_j);

    // End of InitializeConditions for RandomNumber: '<S458>/Random Number3'

    // InitializeConditions for Delay: '<S475>/Delay1'
    Parker_Velocity_Control_DW.icLoad_lz = 1U;

    // InitializeConditions for RandomNumber: '<S458>/Random Number5'
    tmp = floor(Parker_Velocity_Control_P.RandomNumber5_Seed);
    if (rtIsNaN(tmp) || rtIsInf(tmp)) {
      tmp = 0.0;
    } else {
      tmp = fmod(tmp, 4.294967296E+9);
    }

    tseed = tmp < 0.0 ? static_cast<uint32_T>(-static_cast<int32_T>(static_cast<
      uint32_T>(-tmp))) : static_cast<uint32_T>(tmp);
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

    Parker_Velocity_Control_DW.RandSeed_c = tseed;
    Parker_Velocity_Control_DW.NextOutput_k = Park_rt_nrand_Upu32_Yd_f_pw_snf
      (&Parker_Velocity_Control_DW.RandSeed_c) *
      Parker_Velocity_Control_P.RandomNumber5_StdDev +
      Parker_Velocity_Control_P.RandomNumber5_Mean;

    // End of InitializeConditions for RandomNumber: '<S458>/Random Number5'

    // InitializeConditions for DiscreteIntegrator: '<S459>/Discrete-Time Integrator' 
    Parker_Velocity_Control_DW.DiscreteTimeIntegrator_PrevRese = 2;
    Parker_Velocity_Control_DW.DiscreteTimeIntegrator_IC_LOADI = 1U;

    // InitializeConditions for DiscreteIntegrator: '<S459>/Discrete-Time Integrator1' 
    Parker_Velocity_Control_DW.DiscreteTimeIntegrator1_PrevRes = 2;
    Parker_Velocity_Control_DW.DiscreteTimeIntegrator1_IC_LOAD = 1U;

    // InitializeConditions for DiscreteIntegrator: '<S459>/Discrete-Time Integrator2' 
    Parker_Velocity_Control_DW.DiscreteTimeIntegrator2_PrevRes = 2;
    Parker_Velocity_Control_DW.DiscreteTimeIntegrator2_IC_LOAD = 1U;

    // SystemInitialize for IfAction SubSystem: '<S462>/Hold this value'
    // SystemInitialize for Outport: '<S468>/Out1'
    Parker_Velocity_Control_B.In1_aw = Parker_Velocity_Control_P.Out1_Y0_i;

    // End of SystemInitialize for SubSystem: '<S462>/Hold this value'

    // SystemInitialize for IfAction SubSystem: '<S463>/Hold this value'
    // SystemInitialize for Outport: '<S469>/Out1'
    Parker_Velocity_Control_B.In1_b2 = Parker_Velocity_Control_P.Out1_Y0_bu;

    // End of SystemInitialize for SubSystem: '<S463>/Hold this value'

    // SystemInitialize for IfAction SubSystem: '<S464>/Hold this value'
    // SystemInitialize for Outport: '<S470>/Out1'
    Parker_Velocity_Control_B.In1_kz = Parker_Velocity_Control_P.Out1_Y0_j;

    // End of SystemInitialize for SubSystem: '<S464>/Hold this value'

    // SystemInitialize for IfAction SubSystem: '<S465>/Hold this value'
    // SystemInitialize for Outport: '<S471>/Out1'
    Parker_Velocity_Control_B.In1_id = Parker_Velocity_Control_P.Out1_Y0_mq;

    // End of SystemInitialize for SubSystem: '<S465>/Hold this value'

    // SystemInitialize for IfAction SubSystem: '<S466>/Hold this value'
    // SystemInitialize for Outport: '<S472>/Out1'
    Parker_Velocity_Control_B.In1_es = Parker_Velocity_Control_P.Out1_Y0_e;

    // End of SystemInitialize for SubSystem: '<S466>/Hold this value'

    // SystemInitialize for IfAction SubSystem: '<S474>/Hold this value'
    // SystemInitialize for Outport: '<S480>/Out1'
    Parker_Velocity_Control_B.In1_pr = Parker_Velocity_Control_P.Out1_Y0_eo;

    // End of SystemInitialize for SubSystem: '<S474>/Hold this value'

    // SystemInitialize for IfAction SubSystem: '<S475>/Hold this value'
    // SystemInitialize for Outport: '<S481>/Out1'
    Parker_Velocity_Control_B.In1_fv = Parker_Velocity_Control_P.Out1_Y0_d;

    // End of SystemInitialize for SubSystem: '<S475>/Hold this value'

    // SystemInitialize for IfAction SubSystem: '<S478>/Hold this value'
    // SystemInitialize for Outport: '<S484>/Out1'
    Parker_Velocity_Control_B.In1_adc = Parker_Velocity_Control_P.Out1_Y0_ia;

    // End of SystemInitialize for SubSystem: '<S478>/Hold this value'

    // SystemInitialize for IfAction SubSystem: '<S460>/RED + ARM'
    for (i = 0; i < 12; i++) {
      // InitializeConditions for Delay: '<S490>/Delay'
      Parker_Velocity_Control_DW.Delay_DSTATE[i] =
        Parker_Velocity_Control_P.Delay_InitialCondition_k;

      // InitializeConditions for DiscreteIntegrator: '<S490>/Acceleration  to Velocity' 
      Parker_Velocity_Control_DW.AccelerationtoVelocity_DSTATE_o[i] =
        Parker_Velocity_Control_P.AccelerationtoVelocity_IC[i];
    }

    // InitializeConditions for Delay: '<S495>/Delay1'
    Parker_Velocity_Control_DW.icLoad_nt = 1U;

    // InitializeConditions for RandomNumber: '<S490>/Random Number2'
    tmp = floor(Parker_Velocity_Control_P.RandomNumber2_Seed);
    if (rtIsNaN(tmp) || rtIsInf(tmp)) {
      tmp = 0.0;
    } else {
      tmp = fmod(tmp, 4.294967296E+9);
    }

    tseed = tmp < 0.0 ? static_cast<uint32_T>(-static_cast<int32_T>(static_cast<
      uint32_T>(-tmp))) : static_cast<uint32_T>(tmp);
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

    Parker_Velocity_Control_DW.RandSeed_o = tseed;
    Parker_Velocity_Control_DW.NextOutput_c2 = Park_rt_nrand_Upu32_Yd_f_pw_snf
      (&Parker_Velocity_Control_DW.RandSeed_o) *
      Parker_Velocity_Control_P.RandomNumber2_StdDev +
      Parker_Velocity_Control_P.RandomNumber2_Mean;

    // End of InitializeConditions for RandomNumber: '<S490>/Random Number2'

    // InitializeConditions for Delay: '<S494>/Delay1'
    Parker_Velocity_Control_DW.icLoad_el = 1U;

    // InitializeConditions for RandomNumber: '<S490>/Random Number1'
    tmp = floor(Parker_Velocity_Control_P.RandomNumber1_Seed);
    if (rtIsNaN(tmp) || rtIsInf(tmp)) {
      tmp = 0.0;
    } else {
      tmp = fmod(tmp, 4.294967296E+9);
    }

    tseed = tmp < 0.0 ? static_cast<uint32_T>(-static_cast<int32_T>(static_cast<
      uint32_T>(-tmp))) : static_cast<uint32_T>(tmp);
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

    Parker_Velocity_Control_DW.RandSeed_p = tseed;
    Parker_Velocity_Control_DW.NextOutput_lz = Park_rt_nrand_Upu32_Yd_f_pw_snf
      (&Parker_Velocity_Control_DW.RandSeed_p) *
      Parker_Velocity_Control_P.RandomNumber1_StdDev +
      Parker_Velocity_Control_P.RandomNumber1_Mean;

    // End of InitializeConditions for RandomNumber: '<S490>/Random Number1'

    // InitializeConditions for RandomNumber: '<S490>/Random Number3'
    tmp = floor(Parker_Velocity_Control_P.RandomNumber3_Seed);
    if (rtIsNaN(tmp) || rtIsInf(tmp)) {
      tmp = 0.0;
    } else {
      tmp = fmod(tmp, 4.294967296E+9);
    }

    tseed = tmp < 0.0 ? static_cast<uint32_T>(-static_cast<int32_T>(static_cast<
      uint32_T>(-tmp))) : static_cast<uint32_T>(tmp);
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

    Parker_Velocity_Control_DW.RandSeed_h = tseed;
    Parker_Velocity_Control_DW.NextOutput_dv = Park_rt_nrand_Upu32_Yd_f_pw_snf
      (&Parker_Velocity_Control_DW.RandSeed_h) *
      Parker_Velocity_Control_P.RandomNumber3_StdDev +
      Parker_Velocity_Control_P.RandomNumber3_Mean;

    // End of InitializeConditions for RandomNumber: '<S490>/Random Number3'

    // SystemInitialize for IfAction SubSystem: '<S494>/Hold this value'
    // SystemInitialize for Outport: '<S496>/Out1'
    Parker_Velocity_Control_B.In1_lg = Parker_Velocity_Control_P.Out1_Y0_h;

    // End of SystemInitialize for SubSystem: '<S494>/Hold this value'

    // SystemInitialize for IfAction SubSystem: '<S495>/Hold this value'
    // SystemInitialize for Outport: '<S497>/Out1'
    Parker_Velocity_Control_B.In1_h = Parker_Velocity_Control_P.Out1_Y0_hv;

    // End of SystemInitialize for SubSystem: '<S495>/Hold this value'
    // End of SystemInitialize for SubSystem: '<S460>/RED + ARM'

    // SystemInitialize for IfAction SubSystem: '<S460>/RED Only'
    // InitializeConditions for DiscreteIntegrator: '<S499>/Acceleration  to Velocity' 
    Parker_Velocity_Control_DW.AccelerationtoVelocity_DSTATE_m[0] =
      Parker_Velocity_Control_P.AccelerationtoVelocity_IC_c;

    // InitializeConditions for DiscreteIntegrator: '<S499>/Velocity to Position' 
    Parker_Velocity_Control_DW.VelocitytoPosition_DSTATE_f[0] =
      Parker_Velocity_Control_P.drop_states_RED[0];

    // InitializeConditions for DiscreteIntegrator: '<S499>/Acceleration  to Velocity' 
    Parker_Velocity_Control_DW.AccelerationtoVelocity_DSTATE_m[1] =
      Parker_Velocity_Control_P.AccelerationtoVelocity_IC_c;

    // InitializeConditions for DiscreteIntegrator: '<S499>/Velocity to Position' 
    Parker_Velocity_Control_DW.VelocitytoPosition_DSTATE_f[1] =
      Parker_Velocity_Control_P.drop_states_RED[1];

    // InitializeConditions for DiscreteIntegrator: '<S499>/Acceleration  to Velocity' 
    Parker_Velocity_Control_DW.AccelerationtoVelocity_DSTATE_m[2] =
      Parker_Velocity_Control_P.AccelerationtoVelocity_IC_c;

    // InitializeConditions for DiscreteIntegrator: '<S499>/Velocity to Position' 
    Parker_Velocity_Control_DW.VelocitytoPosition_DSTATE_f[2] =
      Parker_Velocity_Control_P.drop_states_RED[2];

    // InitializeConditions for RandomNumber: '<S499>/Random Number'
    tmp = floor(Parker_Velocity_Control_P.RandomNumber_Seed);
    if (rtIsNaN(tmp) || rtIsInf(tmp)) {
      tmp = 0.0;
    } else {
      tmp = fmod(tmp, 4.294967296E+9);
    }

    tseed = tmp < 0.0 ? static_cast<uint32_T>(-static_cast<int32_T>(static_cast<
      uint32_T>(-tmp))) : static_cast<uint32_T>(tmp);
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

    Parker_Velocity_Control_DW.RandSeed_mt = tseed;
    Parker_Velocity_Control_DW.NextOutput_lo = Park_rt_nrand_Upu32_Yd_f_pw_snf
      (&Parker_Velocity_Control_DW.RandSeed_mt) * sqrt
      (Parker_Velocity_Control_P.noise_variance_RED) +
      Parker_Velocity_Control_P.RandomNumber_Mean;

    // End of InitializeConditions for RandomNumber: '<S499>/Random Number'

    // InitializeConditions for Delay: '<S501>/Delay1'
    Parker_Velocity_Control_DW.icLoad_ec = 1U;

    // InitializeConditions for Delay: '<S503>/Delay1'
    Parker_Velocity_Control_DW.icLoad_by = 1U;

    // InitializeConditions for RandomNumber: '<S498>/Random Number7'
    tmp = floor(Parker_Velocity_Control_P.RandomNumber7_Seed);
    if (rtIsNaN(tmp) || rtIsInf(tmp)) {
      tmp = 0.0;
    } else {
      tmp = fmod(tmp, 4.294967296E+9);
    }

    tseed = tmp < 0.0 ? static_cast<uint32_T>(-static_cast<int32_T>(static_cast<
      uint32_T>(-tmp))) : static_cast<uint32_T>(tmp);
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

    Parker_Velocity_Control_DW.RandSeed_ka = tseed;
    Parker_Velocity_Control_DW.NextOutput_de = Park_rt_nrand_Upu32_Yd_f_pw_snf
      (&Parker_Velocity_Control_DW.RandSeed_ka) *
      Parker_Velocity_Control_P.RandomNumber7_StdDev +
      Parker_Velocity_Control_P.RandomNumber7_Mean;

    // End of InitializeConditions for RandomNumber: '<S498>/Random Number7'

    // InitializeConditions for Delay: '<S500>/Delay1'
    Parker_Velocity_Control_DW.icLoad_oe = 1U;

    // InitializeConditions for Delay: '<S504>/Delay1'
    Parker_Velocity_Control_DW.icLoad_mq = 1U;

    // InitializeConditions for RandomNumber: '<S498>/Random Number6'
    tmp = floor(Parker_Velocity_Control_P.RandomNumber6_Seed);
    if (rtIsNaN(tmp) || rtIsInf(tmp)) {
      tmp = 0.0;
    } else {
      tmp = fmod(tmp, 4.294967296E+9);
    }

    tseed = tmp < 0.0 ? static_cast<uint32_T>(-static_cast<int32_T>(static_cast<
      uint32_T>(-tmp))) : static_cast<uint32_T>(tmp);
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

    Parker_Velocity_Control_DW.RandSeed_g = tseed;
    Parker_Velocity_Control_DW.NextOutput_c = Park_rt_nrand_Upu32_Yd_f_pw_snf
      (&Parker_Velocity_Control_DW.RandSeed_g) *
      Parker_Velocity_Control_P.RandomNumber6_StdDev +
      Parker_Velocity_Control_P.RandomNumber6_Mean;

    // End of InitializeConditions for RandomNumber: '<S498>/Random Number6'

    // InitializeConditions for Delay: '<S502>/Delay1'
    Parker_Velocity_Control_DW.icLoad_ov = 1U;

    // InitializeConditions for RandomNumber: '<S498>/Random Number8'
    tmp = floor(Parker_Velocity_Control_P.RandomNumber8_Seed);
    if (rtIsNaN(tmp) || rtIsInf(tmp)) {
      tmp = 0.0;
    } else {
      tmp = fmod(tmp, 4.294967296E+9);
    }

    tseed = tmp < 0.0 ? static_cast<uint32_T>(-static_cast<int32_T>(static_cast<
      uint32_T>(-tmp))) : static_cast<uint32_T>(tmp);
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

    Parker_Velocity_Control_DW.RandSeed_f = tseed;
    Parker_Velocity_Control_DW.NextOutput_h = Park_rt_nrand_Upu32_Yd_f_pw_snf
      (&Parker_Velocity_Control_DW.RandSeed_f) *
      Parker_Velocity_Control_P.RandomNumber8_StdDev +
      Parker_Velocity_Control_P.RandomNumber8_Mean;

    // End of InitializeConditions for RandomNumber: '<S498>/Random Number8'

    // SystemInitialize for IfAction SubSystem: '<S500>/Hold this value'
    // SystemInitialize for Outport: '<S506>/Out1'
    Parker_Velocity_Control_B.In1_kj = Parker_Velocity_Control_P.Out1_Y0_l;

    // End of SystemInitialize for SubSystem: '<S500>/Hold this value'

    // SystemInitialize for IfAction SubSystem: '<S501>/Hold this value'
    // SystemInitialize for Outport: '<S507>/Out1'
    Parker_Velocity_Control_B.In1_ad = Parker_Velocity_Control_P.Out1_Y0_f2;

    // End of SystemInitialize for SubSystem: '<S501>/Hold this value'

    // SystemInitialize for IfAction SubSystem: '<S502>/Hold this value'
    // SystemInitialize for Outport: '<S508>/Out1'
    Parker_Velocity_Control_B.In1_k = Parker_Velocity_Control_P.Out1_Y0_fu;

    // End of SystemInitialize for SubSystem: '<S502>/Hold this value'

    // SystemInitialize for IfAction SubSystem: '<S503>/Hold this value'
    // SystemInitialize for Outport: '<S509>/Out1'
    Parker_Velocity_Control_B.In1_pk = Parker_Velocity_Control_P.Out1_Y0_lh;

    // End of SystemInitialize for SubSystem: '<S503>/Hold this value'

    // SystemInitialize for IfAction SubSystem: '<S504>/Hold this value'
    // SystemInitialize for Outport: '<S510>/Out1'
    Parker_Velocity_Control_B.In1_lk = Parker_Velocity_Control_P.Out1_Y0_a3;

    // End of SystemInitialize for SubSystem: '<S504>/Hold this value'
    // End of SystemInitialize for SubSystem: '<S460>/RED Only'
    // End of SystemInitialize for SubSystem: '<Root>/Simulate Plant Dynamics'

    // SystemInitialize for IfAction SubSystem: '<S2>/Change Behavior'
    // Start for MATLABSystem: '<S23>/Digital Write'
    Parker_Velocity_Control_DW.obj_i.matlabCodegenIsDeleted = true;
    Parker_Velocity_Control_DW.obj_i.isInitialized = 0;
    Parker_Velocity_Control_DW.obj_i.matlabCodegenIsDeleted = false;
    Parker_Velocity_Control_DW.obj_i.isSetupComplete = false;
    Parker_Velocity_Control_DW.obj_i.isInitialized = 1;
    MW_gpioInit(10U, true);
    Parker_Velocity_Control_DW.obj_i.isSetupComplete = true;

    // Start for MATLABSystem: '<S24>/Digital Write'
    Parker_Velocity_Control_DW.obj_a.matlabCodegenIsDeleted = true;
    Parker_Velocity_Control_DW.obj_a.isInitialized = 0;
    Parker_Velocity_Control_DW.obj_a.matlabCodegenIsDeleted = false;
    Parker_Velocity_Control_DW.obj_a.isSetupComplete = false;
    Parker_Velocity_Control_DW.obj_a.isInitialized = 1;
    MW_gpioInit(26U, true);
    Parker_Velocity_Control_DW.obj_a.isSetupComplete = true;

    // End of SystemInitialize for SubSystem: '<S2>/Change Behavior'

    // SystemInitialize for Atomic SubSystem: '<S4>/Send Commands to PWM Blocks' 
    // Start for MATLABSystem: '<S33>/RED T1 - BLACK T8'
    Parker_Velocity_Control_DW.obj_m.matlabCodegenIsDeleted = true;
    Parker_Velocity_Control_DW.obj_m.isInitialized = 0;
    Parker_Velocity_Control_DW.obj_m.matlabCodegenIsDeleted = false;
    Parker_Velocity_Control_DW.obj_m.isSetupComplete = false;
    Parker_Velocity_Control_DW.obj_m.isInitialized = 1;
    Parker_Velocity_Control_DW.obj_m.PinNumber = 27U;
    i = EXT_PWMBlock_init(Parker_Velocity_Control_DW.obj_m.PinNumber, 10U, 0.0);
    if (i != 0) {
      for (i = 0; i < 45; i++) {
        Parker_Velocity_Control_B.cv[i] = tmp_0[i];
      }

      printf(Parker_Velocity_Control_B.cv,
             Parker_Velocity_Control_DW.obj_m.PinNumber);
    }

    Parker_Velocity_Control_DW.obj_m.isSetupComplete = true;

    // End of Start for MATLABSystem: '<S33>/RED T1 - BLACK T8'

    // Start for MATLABSystem: '<S33>/RED T2 - BLACK T3'
    Parker_Velocity_Control_DW.obj_ma.matlabCodegenIsDeleted = true;
    Parker_Velocity_Control_DW.obj_ma.isInitialized = 0;
    Parker_Velocity_Control_DW.obj_ma.matlabCodegenIsDeleted = false;
    Parker_Velocity_Control_DW.obj_ma.isSetupComplete = false;
    Parker_Velocity_Control_DW.obj_ma.isInitialized = 1;
    Parker_Velocity_Control_DW.obj_ma.PinNumber = 19U;
    i = EXT_PWMBlock_init(Parker_Velocity_Control_DW.obj_ma.PinNumber, 10U, 0.0);
    if (i != 0) {
      for (i = 0; i < 45; i++) {
        Parker_Velocity_Control_B.cv[i] = tmp_0[i];
      }

      printf(Parker_Velocity_Control_B.cv,
             Parker_Velocity_Control_DW.obj_ma.PinNumber);
    }

    Parker_Velocity_Control_DW.obj_ma.isSetupComplete = true;

    // End of Start for MATLABSystem: '<S33>/RED T2 - BLACK T3'

    // Start for MATLABSystem: '<S33>/RED T3'
    Parker_Velocity_Control_DW.obj_h.matlabCodegenIsDeleted = true;
    Parker_Velocity_Control_DW.obj_h.isInitialized = 0;
    Parker_Velocity_Control_DW.obj_h.matlabCodegenIsDeleted = false;
    Parker_Velocity_Control_DW.obj_h.isSetupComplete = false;
    Parker_Velocity_Control_DW.obj_h.isInitialized = 1;
    Parker_Velocity_Control_DW.obj_h.PinNumber = 4U;
    i = EXT_PWMBlock_init(Parker_Velocity_Control_DW.obj_h.PinNumber, 10U, 0.0);
    if (i != 0) {
      for (i = 0; i < 45; i++) {
        Parker_Velocity_Control_B.cv[i] = tmp_0[i];
      }

      printf(Parker_Velocity_Control_B.cv,
             Parker_Velocity_Control_DW.obj_h.PinNumber);
    }

    Parker_Velocity_Control_DW.obj_h.isSetupComplete = true;

    // End of Start for MATLABSystem: '<S33>/RED T3'

    // Start for MATLABSystem: '<S33>/RED T4 - BLACK T5'
    Parker_Velocity_Control_DW.obj_k.matlabCodegenIsDeleted = true;
    Parker_Velocity_Control_DW.obj_k.isInitialized = 0;
    Parker_Velocity_Control_DW.obj_k.matlabCodegenIsDeleted = false;
    Parker_Velocity_Control_DW.obj_k.isSetupComplete = false;
    Parker_Velocity_Control_DW.obj_k.isInitialized = 1;
    Parker_Velocity_Control_DW.obj_k.PinNumber = 22U;
    i = EXT_PWMBlock_init(Parker_Velocity_Control_DW.obj_k.PinNumber, 10U, 0.0);
    if (i != 0) {
      for (i = 0; i < 45; i++) {
        Parker_Velocity_Control_B.cv[i] = tmp_0[i];
      }

      printf(Parker_Velocity_Control_B.cv,
             Parker_Velocity_Control_DW.obj_k.PinNumber);
    }

    Parker_Velocity_Control_DW.obj_k.isSetupComplete = true;

    // End of Start for MATLABSystem: '<S33>/RED T4 - BLACK T5'

    // Start for MATLABSystem: '<S33>/RED T5 - BLACK T4'
    Parker_Velocity_Control_DW.obj_hw.matlabCodegenIsDeleted = true;
    Parker_Velocity_Control_DW.obj_hw.isInitialized = 0;
    Parker_Velocity_Control_DW.obj_hw.matlabCodegenIsDeleted = false;
    Parker_Velocity_Control_DW.obj_hw.isSetupComplete = false;
    Parker_Velocity_Control_DW.obj_hw.isInitialized = 1;
    Parker_Velocity_Control_DW.obj_hw.PinNumber = 5U;
    i = EXT_PWMBlock_init(Parker_Velocity_Control_DW.obj_hw.PinNumber, 10U, 0.0);
    if (i != 0) {
      for (i = 0; i < 45; i++) {
        Parker_Velocity_Control_B.cv[i] = tmp_0[i];
      }

      printf(Parker_Velocity_Control_B.cv,
             Parker_Velocity_Control_DW.obj_hw.PinNumber);
    }

    Parker_Velocity_Control_DW.obj_hw.isSetupComplete = true;

    // End of Start for MATLABSystem: '<S33>/RED T5 - BLACK T4'

    // Start for MATLABSystem: '<S33>/RED T6 - BLACK T7'
    Parker_Velocity_Control_DW.obj_hf.matlabCodegenIsDeleted = true;
    Parker_Velocity_Control_DW.obj_hf.isInitialized = 0;
    Parker_Velocity_Control_DW.obj_hf.matlabCodegenIsDeleted = false;
    Parker_Velocity_Control_DW.obj_hf.isSetupComplete = false;
    Parker_Velocity_Control_DW.obj_hf.isInitialized = 1;
    Parker_Velocity_Control_DW.obj_hf.PinNumber = 6U;
    i = EXT_PWMBlock_init(Parker_Velocity_Control_DW.obj_hf.PinNumber, 10U, 0.0);
    if (i != 0) {
      for (i = 0; i < 45; i++) {
        Parker_Velocity_Control_B.cv[i] = tmp_0[i];
      }

      printf(Parker_Velocity_Control_B.cv,
             Parker_Velocity_Control_DW.obj_hf.PinNumber);
    }

    Parker_Velocity_Control_DW.obj_hf.isSetupComplete = true;

    // End of Start for MATLABSystem: '<S33>/RED T6 - BLACK T7'

    // Start for MATLABSystem: '<S33>/RED T7 - BLACK T6'
    Parker_Velocity_Control_DW.obj_ce.matlabCodegenIsDeleted = true;
    Parker_Velocity_Control_DW.obj_ce.isInitialized = 0;
    Parker_Velocity_Control_DW.obj_ce.matlabCodegenIsDeleted = false;
    Parker_Velocity_Control_DW.obj_ce.isSetupComplete = false;
    Parker_Velocity_Control_DW.obj_ce.isInitialized = 1;
    Parker_Velocity_Control_DW.obj_ce.PinNumber = 13U;
    i = EXT_PWMBlock_init(Parker_Velocity_Control_DW.obj_ce.PinNumber, 10U, 0.0);
    if (i != 0) {
      for (i = 0; i < 45; i++) {
        Parker_Velocity_Control_B.cv[i] = tmp_0[i];
      }

      printf(Parker_Velocity_Control_B.cv,
             Parker_Velocity_Control_DW.obj_ce.PinNumber);
    }

    Parker_Velocity_Control_DW.obj_ce.isSetupComplete = true;

    // End of Start for MATLABSystem: '<S33>/RED T7 - BLACK T6'

    // Start for MATLABSystem: '<S33>/RED T8 - BLACK T1'
    Parker_Velocity_Control_DW.obj_fo.matlabCodegenIsDeleted = true;
    Parker_Velocity_Control_DW.obj_fo.isInitialized = 0;
    Parker_Velocity_Control_DW.obj_fo.matlabCodegenIsDeleted = false;
    Parker_Velocity_Control_DW.obj_fo.isSetupComplete = false;
    Parker_Velocity_Control_DW.obj_fo.isInitialized = 1;
    Parker_Velocity_Control_DW.obj_fo.PinNumber = 17U;
    i = EXT_PWMBlock_init(Parker_Velocity_Control_DW.obj_fo.PinNumber, 10U, 0.0);
    if (i != 0) {
      for (i = 0; i < 45; i++) {
        Parker_Velocity_Control_B.cv[i] = tmp_0[i];
      }

      printf(Parker_Velocity_Control_B.cv,
             Parker_Velocity_Control_DW.obj_fo.PinNumber);
    }

    Parker_Velocity_Control_DW.obj_fo.isSetupComplete = true;

    // End of Start for MATLABSystem: '<S33>/RED T8 - BLACK T1'

    // Start for MATLABSystem: '<S33>/BLACK T2'
    Parker_Velocity_Control_DW.obj_ci.matlabCodegenIsDeleted = true;
    Parker_Velocity_Control_DW.obj_ci.isInitialized = 0;
    Parker_Velocity_Control_DW.obj_ci.matlabCodegenIsDeleted = false;
    Parker_Velocity_Control_DW.obj_ci.isSetupComplete = false;
    Parker_Velocity_Control_DW.obj_ci.isInitialized = 1;
    Parker_Velocity_Control_DW.obj_ci.PinNumber = 9U;
    i = EXT_PWMBlock_init(Parker_Velocity_Control_DW.obj_ci.PinNumber, 10U, 0.0);
    if (i != 0) {
      for (i = 0; i < 45; i++) {
        Parker_Velocity_Control_B.cv[i] = tmp_0[i];
      }

      printf(Parker_Velocity_Control_B.cv,
             Parker_Velocity_Control_DW.obj_ci.PinNumber);
    }

    Parker_Velocity_Control_DW.obj_ci.isSetupComplete = true;

    // End of Start for MATLABSystem: '<S33>/BLACK T2'
    // End of SystemInitialize for SubSystem: '<S4>/Send Commands to PWM Blocks' 

    // SystemInitialize for IfAction SubSystem: '<S4>/Change BLACK Behavior'
    // SystemInitialize for Outport: '<S29>/BLACK PWM'
    for (i = 0; i < 8; i++) {
      Parker_Velocity_Control_B.sf_MATLABFunction2.ThrustPer_Final[i] =
        Parker_Velocity_Control_P.BLACKPWM_Y0;
    }

    // End of SystemInitialize for Outport: '<S29>/BLACK PWM'
    // End of SystemInitialize for SubSystem: '<S4>/Change BLACK Behavior'

    // SystemInitialize for IfAction SubSystem: '<S4>/Change BLUE Behavior'
    // SystemInitialize for Outport: '<S30>/BLUE PWM'
    for (i = 0; i < 8; i++) {
      Parker_Velocity_Control_B.sf_MATLABFunction2_n.ThrustPer_Final[i] =
        Parker_Velocity_Control_P.BLUEPWM_Y0;
    }

    // End of SystemInitialize for Outport: '<S30>/BLUE PWM'
    // End of SystemInitialize for SubSystem: '<S4>/Change BLUE Behavior'

    // SystemInitialize for IfAction SubSystem: '<S4>/Change RED Behavior'
    // SystemInitialize for Outport: '<S31>/RED PWM'
    for (i = 0; i < 8; i++) {
      Parker_Velocity_Control_B.sf_MATLABFunction2_l.ThrustPer_Final[i] =
        Parker_Velocity_Control_P.REDPWM_Y0;
    }

    // End of SystemInitialize for Outport: '<S31>/RED PWM'
    // End of SystemInitialize for SubSystem: '<S4>/Change RED Behavior'

    // SystemInitialize for IfAction SubSystem: '<S5>/Change RED Behavior'
    // InitializeConditions for Delay: '<S85>/Delay1'
    Parker_Velocity_Control_DW.Delay1_DSTATE_kt =
      Parker_Velocity_Control_P.Delay1_InitialCondition_m;

    // InitializeConditions for Delay: '<S85>/Delay2'
    Parker_Velocity_Control_DW.Delay2_DSTATE_a[0] =
      Parker_Velocity_Control_P.Delay2_InitialCondition_k;
    Parker_Velocity_Control_DW.Delay2_DSTATE_a[1] =
      Parker_Velocity_Control_P.Delay2_InitialCondition_k;

    // InitializeConditions for Delay: '<S85>/Delay5'
    Parker_Velocity_Control_DW.Delay5_DSTATE_g[0] =
      Parker_Velocity_Control_P.Delay5_InitialCondition_e;
    Parker_Velocity_Control_DW.Delay5_DSTATE_g[1] =
      Parker_Velocity_Control_P.Delay5_InitialCondition_e;
    Parker_Velocity_Control_DW.Delay5_DSTATE_g[2] =
      Parker_Velocity_Control_P.Delay5_InitialCondition_e;

    // InitializeConditions for Delay: '<S85>/Delay3'
    Parker_Velocity_Control_DW.Delay3_DSTATE_n[0] =
      Parker_Velocity_Control_P.Delay3_InitialCondition_i;
    Parker_Velocity_Control_DW.Delay3_DSTATE_n[1] =
      Parker_Velocity_Control_P.Delay3_InitialCondition_i;
    Parker_Velocity_Control_DW.Delay3_DSTATE_n[2] =
      Parker_Velocity_Control_P.Delay3_InitialCondition_i;
    Parker_Velocity_Control_DW.Delay3_DSTATE_n[3] =
      Parker_Velocity_Control_P.Delay3_InitialCondition_i;

    // InitializeConditions for Delay: '<S85>/Delay4'
    for (i = 0; i < 5; i++) {
      Parker_Velocity_Control_DW.Delay4_DSTATE_i[i] =
        Parker_Velocity_Control_P.Delay4_InitialCondition_d;
    }

    // End of InitializeConditions for Delay: '<S85>/Delay4'

    // InitializeConditions for Delay: '<S80>/Delay'
    Parker_Velocity_Control_DW.Delay_DSTATE_k =
      Parker_Velocity_Control_P.Delay_InitialCondition;

    // SystemInitialize for Atomic SubSystem: '<S81>/Send Direction to Motor Controller' 
    // Start for MATLABSystem: '<S93>/Digital Write'
    Parker_Velocity_Control_DW.obj_bm.matlabCodegenIsDeleted = true;
    Parker_Velocity_Control_DW.obj_bm.isInitialized = 0;
    Parker_Velocity_Control_DW.obj_bm.matlabCodegenIsDeleted = false;
    Parker_Velocity_Control_DW.obj_bm.isSetupComplete = false;
    Parker_Velocity_Control_DW.obj_bm.isInitialized = 1;
    MW_gpioInit(21U, true);
    Parker_Velocity_Control_DW.obj_bm.isSetupComplete = true;

    // End of SystemInitialize for SubSystem: '<S81>/Send Direction to Motor Controller' 

    // SystemInitialize for Atomic SubSystem: '<S81>/Send PWM to Motor Controller' 
    // Start for MATLABSystem: '<S90>/PWM1'
    Parker_Velocity_Control_DW.obj_d.matlabCodegenIsDeleted = true;
    Parker_Velocity_Control_DW.obj_d.isInitialized = 0;
    Parker_Velocity_Control_DW.obj_d.matlabCodegenIsDeleted = false;
    Parker_Velocity_Control_DW.obj_d.isSetupComplete = false;
    Parker_Velocity_Control_DW.obj_d.isInitialized = 1;
    Parker_Velocity_Control_DW.obj_d.PinNumber = 18U;
    i = EXT_PWMBlock_init(Parker_Velocity_Control_DW.obj_d.PinNumber, 10U, 0.0);
    if (i != 0) {
      for (i = 0; i < 45; i++) {
        Parker_Velocity_Control_B.cv[i] = tmp_0[i];
      }

      printf(Parker_Velocity_Control_B.cv,
             Parker_Velocity_Control_DW.obj_d.PinNumber);
    }

    Parker_Velocity_Control_DW.obj_d.isSetupComplete = true;

    // End of Start for MATLABSystem: '<S90>/PWM1'
    // End of SystemInitialize for SubSystem: '<S81>/Send PWM to Motor Controller' 

    // SystemInitialize for Atomic SubSystem: '<S81>/Turn on Motor'
    // Start for MATLABSystem: '<S94>/Digital Write'
    Parker_Velocity_Control_DW.obj_cp.matlabCodegenIsDeleted = true;
    Parker_Velocity_Control_DW.obj_cp.isInitialized = 0;
    Parker_Velocity_Control_DW.obj_cp.matlabCodegenIsDeleted = false;
    Parker_Velocity_Control_DW.obj_cp.isSetupComplete = false;
    Parker_Velocity_Control_DW.obj_cp.isInitialized = 1;
    MW_gpioInit(25U, true);
    Parker_Velocity_Control_DW.obj_cp.isSetupComplete = true;

    // End of SystemInitialize for SubSystem: '<S81>/Turn on Motor'

    // SystemInitialize for Atomic SubSystem: '<S81>/Obtain RW Status'
    // Start for MATLABSystem: '<S92>/Digital Read'
    Parker_Velocity_Control_DW.obj_f.matlabCodegenIsDeleted = true;
    Parker_Velocity_Control_DW.obj_f.isInitialized = 0;
    Parker_Velocity_Control_DW.obj_f.matlabCodegenIsDeleted = false;
    if (((!rtIsInf(Parker_Velocity_Control_P.DigitalRead_SampleTime)) &&
         (!rtIsNaN(Parker_Velocity_Control_P.DigitalRead_SampleTime))) ||
        rtIsInf(Parker_Velocity_Control_P.DigitalRead_SampleTime)) {
      sampleTime = Parker_Velocity_Control_P.DigitalRead_SampleTime;
    }

    Parker_Velocity_Control_DW.obj_f.SampleTime = sampleTime;
    Parker_Velocity_Control_DW.obj_f.isSetupComplete = false;
    Parker_Velocity_Control_DW.obj_f.isInitialized = 1;
    MW_gpioInit(24U, false);
    Parker_Velocity_Control_DW.obj_f.isSetupComplete = true;

    // End of Start for MATLABSystem: '<S92>/Digital Read'
    // End of SystemInitialize for SubSystem: '<S81>/Obtain RW Status'
    // End of SystemInitialize for SubSystem: '<S5>/Change RED Behavior'

    // SystemInitialize for IfAction SubSystem: '<S6>/Use Hardware to Obtain States' 
    // Start for S-Function (sdspFromNetwork): '<S97>/UDP Receive'
    sErr = GetErrorBuffer(&Parker_Velocity_Control_DW.UDPReceive_NetworkLib[0U]);
    CreateUDPInterface(&Parker_Velocity_Control_DW.UDPReceive_NetworkLib[0U]);
    if (*sErr == 0) {
      LibCreate_Network(&Parker_Velocity_Control_DW.UDPReceive_NetworkLib[0U], 0,
                        "0.0.0.0", Parker_Velocity_Control_P.UDPReceive_Port,
                        "0.0.0.0", -1, 8192, 8, 0);
    }

    if (*sErr == 0) {
      LibStart(&Parker_Velocity_Control_DW.UDPReceive_NetworkLib[0U]);
    }

    if (*sErr != 0) {
      DestroyUDPInterface(&Parker_Velocity_Control_DW.UDPReceive_NetworkLib[0U]);
      if (*sErr != 0) {
        rtmSetErrorStatus(Parker_Velocity_Control_M, sErr);
        rtmSetStopRequested(Parker_Velocity_Control_M, 1);
      }
    }

    // End of Start for S-Function (sdspFromNetwork): '<S97>/UDP Receive'

    // InitializeConditions for Delay: '<S107>/Delay1'
    Parker_Velocity_Control_DW.icLoad_c = 1U;

    // InitializeConditions for Delay: '<S99>/Delay1'
    Parker_Velocity_Control_DW.icLoad_h0 = 1U;

    // InitializeConditions for Delay: '<S108>/Delay1'
    Parker_Velocity_Control_DW.icLoad_kt = 1U;

    // InitializeConditions for Delay: '<S100>/Delay1'
    Parker_Velocity_Control_DW.icLoad_ktm = 1U;

    // InitializeConditions for S-Function (sdspunwrap2): '<S97>/Unwrap1'
    Parker_Velocity_Control_DW.Unwrap1_FirstStep = true;

    // InitializeConditions for Delay: '<S109>/Delay1'
    Parker_Velocity_Control_DW.icLoad_gn = 1U;

    // InitializeConditions for Delay: '<S101>/Delay1'
    Parker_Velocity_Control_DW.icLoad_pz = 1U;

    // InitializeConditions for Delay: '<S102>/Delay1'
    Parker_Velocity_Control_DW.icLoad_fy = 1U;

    // InitializeConditions for Delay: '<S98>/Delay1'
    Parker_Velocity_Control_DW.icLoad_ea = 1U;

    // InitializeConditions for Delay: '<S110>/Delay1'
    Parker_Velocity_Control_DW.icLoad_df = 1U;

    // InitializeConditions for Delay: '<S105>/Delay1'
    Parker_Velocity_Control_DW.icLoad_pj = 1U;

    // InitializeConditions for Delay: '<S111>/Delay1'
    Parker_Velocity_Control_DW.icLoad_ap = 1U;

    // InitializeConditions for S-Function (sdspunwrap2): '<S97>/Unwrap'
    Parker_Velocity_Control_DW.Unwrap_FirstStep = true;

    // InitializeConditions for Delay: '<S106>/Delay1'
    Parker_Velocity_Control_DW.icLoad_hr = 1U;

    // InitializeConditions for Delay: '<S112>/Delay1'
    Parker_Velocity_Control_DW.icLoad_al = 1U;

    // InitializeConditions for Delay: '<S103>/Delay1'
    Parker_Velocity_Control_DW.icLoad_dt = 1U;

    // InitializeConditions for S-Function (sdspunwrap2): '<S97>/Unwrap2'
    Parker_Velocity_Control_DW.Unwrap2_FirstStep = true;

    // InitializeConditions for Delay: '<S104>/Delay1'
    Parker_Velocity_Control_DW.icLoad_l1b = 1U;

    // SystemInitialize for IfAction SubSystem: '<S98>/Hold this value'
    // SystemInitialize for Outport: '<S113>/Out1'
    Parker_Velocity_Control_B.In1_ky = Parker_Velocity_Control_P.Out1_Y0;

    // End of SystemInitialize for SubSystem: '<S98>/Hold this value'

    // SystemInitialize for IfAction SubSystem: '<S99>/Hold this value'
    // SystemInitialize for Outport: '<S114>/Out1'
    Parker_Velocity_Control_B.In1_cw = Parker_Velocity_Control_P.Out1_Y0_p;

    // End of SystemInitialize for SubSystem: '<S99>/Hold this value'

    // SystemInitialize for IfAction SubSystem: '<S100>/Hold this value'
    // SystemInitialize for Outport: '<S115>/Out1'
    Parker_Velocity_Control_B.In1_gp = Parker_Velocity_Control_P.Out1_Y0_a;

    // End of SystemInitialize for SubSystem: '<S100>/Hold this value'

    // SystemInitialize for IfAction SubSystem: '<S101>/Hold this value'
    // SystemInitialize for Outport: '<S116>/Out1'
    Parker_Velocity_Control_B.In1_n = Parker_Velocity_Control_P.Out1_Y0_pf;

    // End of SystemInitialize for SubSystem: '<S101>/Hold this value'

    // SystemInitialize for IfAction SubSystem: '<S102>/Hold this value'
    // SystemInitialize for Outport: '<S117>/Out1'
    Parker_Velocity_Control_B.In1_i0 = Parker_Velocity_Control_P.Out1_Y0_k;

    // End of SystemInitialize for SubSystem: '<S102>/Hold this value'

    // SystemInitialize for IfAction SubSystem: '<S103>/Hold this value'
    // SystemInitialize for Outport: '<S118>/Out1'
    Parker_Velocity_Control_B.In1_as = Parker_Velocity_Control_P.Out1_Y0_f;

    // End of SystemInitialize for SubSystem: '<S103>/Hold this value'

    // SystemInitialize for IfAction SubSystem: '<S104>/Hold this value'
    // SystemInitialize for Outport: '<S119>/Out1'
    Parker_Velocity_Control_B.In1_lu = Parker_Velocity_Control_P.Out1_Y0_m;

    // End of SystemInitialize for SubSystem: '<S104>/Hold this value'

    // SystemInitialize for IfAction SubSystem: '<S105>/Hold this value'
    // SystemInitialize for Outport: '<S120>/Out1'
    Parker_Velocity_Control_B.In1_f0 = Parker_Velocity_Control_P.Out1_Y0_po;

    // End of SystemInitialize for SubSystem: '<S105>/Hold this value'

    // SystemInitialize for IfAction SubSystem: '<S106>/Hold this value'
    // SystemInitialize for Outport: '<S121>/Out1'
    Parker_Velocity_Control_B.In1_lb = Parker_Velocity_Control_P.Out1_Y0_n;

    // End of SystemInitialize for SubSystem: '<S106>/Hold this value'

    // SystemInitialize for IfAction SubSystem: '<S107>/Hold this value'
    // SystemInitialize for Outport: '<S122>/Out1'
    Parker_Velocity_Control_B.In1_gw = Parker_Velocity_Control_P.Out1_Y0_pl;

    // End of SystemInitialize for SubSystem: '<S107>/Hold this value'

    // SystemInitialize for IfAction SubSystem: '<S108>/Hold this value'
    // SystemInitialize for Outport: '<S123>/Out1'
    Parker_Velocity_Control_B.In1_k5 = Parker_Velocity_Control_P.Out1_Y0_aq;

    // End of SystemInitialize for SubSystem: '<S108>/Hold this value'

    // SystemInitialize for IfAction SubSystem: '<S109>/Hold this value'
    // SystemInitialize for Outport: '<S124>/Out1'
    Parker_Velocity_Control_B.In1_ex = Parker_Velocity_Control_P.Out1_Y0_au;

    // End of SystemInitialize for SubSystem: '<S109>/Hold this value'

    // SystemInitialize for IfAction SubSystem: '<S110>/Hold this value'
    // SystemInitialize for Outport: '<S125>/Out1'
    Parker_Velocity_Control_B.In1_fo = Parker_Velocity_Control_P.Out1_Y0_b;

    // End of SystemInitialize for SubSystem: '<S110>/Hold this value'

    // SystemInitialize for IfAction SubSystem: '<S111>/Hold this value'
    // SystemInitialize for Outport: '<S126>/Out1'
    Parker_Velocity_Control_B.In1_db = Parker_Velocity_Control_P.Out1_Y0_bw;

    // End of SystemInitialize for SubSystem: '<S111>/Hold this value'

    // SystemInitialize for IfAction SubSystem: '<S112>/Hold this value'
    // SystemInitialize for Outport: '<S127>/Out1'
    Parker_Velocity_Control_B.In1_dn = Parker_Velocity_Control_P.Out1_Y0_c;

    // End of SystemInitialize for SubSystem: '<S112>/Hold this value'
    // End of SystemInitialize for SubSystem: '<S6>/Use Hardware to Obtain States' 

    // SystemInitialize for IfAction SubSystem: '<S8>/Change Behavior'
    // InitializeConditions for Delay: '<S130>/Delay1'
    Parker_Velocity_Control_DW.Delay1_DSTATE_jp =
      Parker_Velocity_Control_P.Delay1_InitialCondition;

    // InitializeConditions for Delay: '<S130>/Delay2'
    Parker_Velocity_Control_DW.Delay2_DSTATE[0] =
      Parker_Velocity_Control_P.Delay2_InitialCondition;
    Parker_Velocity_Control_DW.Delay2_DSTATE[1] =
      Parker_Velocity_Control_P.Delay2_InitialCondition;

    // InitializeConditions for Delay: '<S130>/Delay5'
    Parker_Velocity_Control_DW.Delay5_DSTATE[0] =
      Parker_Velocity_Control_P.Delay5_InitialCondition;
    Parker_Velocity_Control_DW.Delay5_DSTATE[1] =
      Parker_Velocity_Control_P.Delay5_InitialCondition;
    Parker_Velocity_Control_DW.Delay5_DSTATE[2] =
      Parker_Velocity_Control_P.Delay5_InitialCondition;

    // InitializeConditions for Delay: '<S130>/Delay3'
    Parker_Velocity_Control_DW.Delay3_DSTATE[0] =
      Parker_Velocity_Control_P.Delay3_InitialCondition;
    Parker_Velocity_Control_DW.Delay3_DSTATE[1] =
      Parker_Velocity_Control_P.Delay3_InitialCondition;
    Parker_Velocity_Control_DW.Delay3_DSTATE[2] =
      Parker_Velocity_Control_P.Delay3_InitialCondition;
    Parker_Velocity_Control_DW.Delay3_DSTATE[3] =
      Parker_Velocity_Control_P.Delay3_InitialCondition;

    // InitializeConditions for Delay: '<S131>/Delay1'
    Parker_Velocity_Control_DW.Delay1_DSTATE_n5 =
      Parker_Velocity_Control_P.Delay1_InitialCondition_j;

    // InitializeConditions for Delay: '<S131>/Delay2'
    Parker_Velocity_Control_DW.Delay2_DSTATE_l[0] =
      Parker_Velocity_Control_P.Delay2_InitialCondition_j;
    Parker_Velocity_Control_DW.Delay2_DSTATE_l[1] =
      Parker_Velocity_Control_P.Delay2_InitialCondition_j;

    // InitializeConditions for Delay: '<S131>/Delay5'
    Parker_Velocity_Control_DW.Delay5_DSTATE_b[0] =
      Parker_Velocity_Control_P.Delay5_InitialCondition_g;
    Parker_Velocity_Control_DW.Delay5_DSTATE_b[1] =
      Parker_Velocity_Control_P.Delay5_InitialCondition_g;
    Parker_Velocity_Control_DW.Delay5_DSTATE_b[2] =
      Parker_Velocity_Control_P.Delay5_InitialCondition_g;

    // InitializeConditions for Delay: '<S131>/Delay3'
    Parker_Velocity_Control_DW.Delay3_DSTATE_o[0] =
      Parker_Velocity_Control_P.Delay3_InitialCondition_k;
    Parker_Velocity_Control_DW.Delay3_DSTATE_o[1] =
      Parker_Velocity_Control_P.Delay3_InitialCondition_k;
    Parker_Velocity_Control_DW.Delay3_DSTATE_o[2] =
      Parker_Velocity_Control_P.Delay3_InitialCondition_k;
    Parker_Velocity_Control_DW.Delay3_DSTATE_o[3] =
      Parker_Velocity_Control_P.Delay3_InitialCondition_k;

    // InitializeConditions for Delay: '<S132>/Delay1'
    Parker_Velocity_Control_DW.Delay1_DSTATE_ny =
      Parker_Velocity_Control_P.Delay1_InitialCondition_n;

    // InitializeConditions for Delay: '<S132>/Delay2'
    Parker_Velocity_Control_DW.Delay2_DSTATE_m[0] =
      Parker_Velocity_Control_P.Delay2_InitialCondition_p;
    Parker_Velocity_Control_DW.Delay2_DSTATE_m[1] =
      Parker_Velocity_Control_P.Delay2_InitialCondition_p;

    // InitializeConditions for Delay: '<S132>/Delay5'
    Parker_Velocity_Control_DW.Delay5_DSTATE_n[0] =
      Parker_Velocity_Control_P.Delay5_InitialCondition_o;
    Parker_Velocity_Control_DW.Delay5_DSTATE_n[1] =
      Parker_Velocity_Control_P.Delay5_InitialCondition_o;
    Parker_Velocity_Control_DW.Delay5_DSTATE_n[2] =
      Parker_Velocity_Control_P.Delay5_InitialCondition_o;

    // InitializeConditions for Delay: '<S132>/Delay3'
    Parker_Velocity_Control_DW.Delay3_DSTATE_h[0] =
      Parker_Velocity_Control_P.Delay3_InitialCondition_e;
    Parker_Velocity_Control_DW.Delay3_DSTATE_h[1] =
      Parker_Velocity_Control_P.Delay3_InitialCondition_e;
    Parker_Velocity_Control_DW.Delay3_DSTATE_h[2] =
      Parker_Velocity_Control_P.Delay3_InitialCondition_e;
    Parker_Velocity_Control_DW.Delay3_DSTATE_h[3] =
      Parker_Velocity_Control_P.Delay3_InitialCondition_e;
    for (i = 0; i < 5; i++) {
      // InitializeConditions for Delay: '<S130>/Delay4'
      Parker_Velocity_Control_DW.Delay4_DSTATE[i] =
        Parker_Velocity_Control_P.Delay4_InitialCondition;

      // InitializeConditions for Delay: '<S131>/Delay4'
      Parker_Velocity_Control_DW.Delay4_DSTATE_c[i] =
        Parker_Velocity_Control_P.Delay4_InitialCondition_m;

      // InitializeConditions for Delay: '<S132>/Delay4'
      Parker_Velocity_Control_DW.Delay4_DSTATE_k[i] =
        Parker_Velocity_Control_P.Delay4_InitialCondition_h;
    }

    // Start for MATLABSystem: '<S128>/Read Joint Positions using  Dynamixel Encoders' 
    Parker_Velocity_Control_DW.obj_c.matlabCodegenIsDeleted = true;
    Parker_Velocity_Control_DW.obj_c.isInitialized = 0;
    Parker_Velocity_Control_DW.obj_c.matlabCodegenIsDeleted = false;
    if (((!rtIsInf(Parker_Velocity_Control_P.baseRate)) && (!rtIsNaN
          (Parker_Velocity_Control_P.baseRate))) || rtIsInf
        (Parker_Velocity_Control_P.baseRate)) {
      Parker_Velocity_Control_B.sampleTime_l =
        Parker_Velocity_Control_P.baseRate;
    }

    Parker_Velocity_Control_DW.obj_c.SampleTime =
      Parker_Velocity_Control_B.sampleTime_l;
    Parker_Velocity_Control_DW.obj_c.isSetupComplete = false;
    Parker_Velocity_Control_DW.obj_c.isInitialized = 1;
    Parker_Velocity_Control_DW.obj_c.isSetupComplete = true;

    // End of Start for MATLABSystem: '<S128>/Read Joint Positions using  Dynamixel Encoders' 
    // End of SystemInitialize for SubSystem: '<S8>/Change Behavior'

    // Start for MATLABSystem: '<Root>/Initialization Block for Speed Control'
    Parker_Velocity_Control_DW.obj.matlabCodegenIsDeleted = true;
    Parker_Velocity_Control_DW.obj.isInitialized = 0;
    Parker_Velocity_Control_DW.obj.matlabCodegenIsDeleted = false;
    Parker_Velocity_Control_DW.obj.P_GAIN =
      Parker_Velocity_Control_P.InitializationBlockforSpeedCont;
    Parker_Velocity_Control_DW.obj.I_GAIN =
      Parker_Velocity_Control_P.InitializationBlockforSpeedCo_d;
    Parker_Velocity_Control_DW.obj.VELOCITY_LIMIT =
      Parker_Velocity_Control_P.InitializationBlockforSpeedCo_k;
    Parker_Velocity_Control_DW.obj.ACCELERATION_TIME =
      Parker_Velocity_Control_P.InitializationBlockforSpeedCo_g;
    Parker_Velocity_Control_DW.obj.isSetupComplete = false;
    Parker_Velocity_Control_DW.obj.isInitialized = 1;
    initialize_dynamixel_speed_control(Parker_Velocity_Control_DW.obj.P_GAIN,
      Parker_Velocity_Control_DW.obj.I_GAIN,
      Parker_Velocity_Control_DW.obj.VELOCITY_LIMIT,
      Parker_Velocity_Control_DW.obj.ACCELERATION_TIME);
    Parker_Velocity_Control_DW.obj.isSetupComplete = true;

    // Start for MATLABSystem: '<S17>/WhoAmI'
    Parker_Velocity_Control_DW.obj_b.matlabCodegenIsDeleted = true;
    Parker_Velocity_Control_DW.obj_b.isInitialized = 0;
    Parker_Velocity_Control_DW.obj_b.matlabCodegenIsDeleted = false;
    if (((!rtIsInf(Parker_Velocity_Control_P.WhoAmI_SampleTime)) && (!rtIsNaN
          (Parker_Velocity_Control_P.WhoAmI_SampleTime))) || rtIsInf
        (Parker_Velocity_Control_P.WhoAmI_SampleTime)) {
      Parker_Velocity_Control_B.sampleTime_g =
        Parker_Velocity_Control_P.WhoAmI_SampleTime;
    }

    Parker_Velocity_Control_DW.obj_b.SampleTime =
      Parker_Velocity_Control_B.sampleTime_g;
    Parker_Velocity_Control_DW.obj_b.isSetupComplete = false;
    Parker_Velocity_Control_DW.obj_b.isInitialized = 1;
    Parker_Velocity_Control_DW.obj_b.isSetupComplete = true;

    // End of Start for MATLABSystem: '<S17>/WhoAmI'
  }
}

// Model terminate function
void Parker_Velocity_Control_terminate(void)
{
  char_T *sErr;

  // Terminate for IfAction SubSystem: '<Root>/Phase #3: Experiment'
  // Terminate for IfAction SubSystem: '<S13>/Change RED Behavior'
  // Terminate for IfAction SubSystem: '<S182>/Sub-Phase #4'
  // Terminate for IfAction SubSystem: '<S337>/RED+ARM'
  // Terminate for MATLABSystem: '<S346>/Send Speed Commands to  Manipulator'
  Parker_Veloci_matlabCodegenHa_g(&Parker_Velocity_Control_DW.obj_o);

  // End of Terminate for SubSystem: '<S337>/RED+ARM'
  // End of Terminate for SubSystem: '<S182>/Sub-Phase #4'
  // End of Terminate for SubSystem: '<S13>/Change RED Behavior'
  // End of Terminate for SubSystem: '<Root>/Phase #3: Experiment'
  // Terminate for IfAction SubSystem: '<S2>/Change Behavior'
  // Terminate for MATLABSystem: '<S23>/Digital Write'
  matlabCodegenHandle_matlabCod_i(&Parker_Velocity_Control_DW.obj_i);

  // Terminate for MATLABSystem: '<S24>/Digital Write'
  matlabCodegenHandle_matlabCo_ir(&Parker_Velocity_Control_DW.obj_a);

  // End of Terminate for SubSystem: '<S2>/Change Behavior'

  // Terminate for IfAction SubSystem: '<S6>/Use Hardware to Obtain States'
  // Terminate for S-Function (sdspFromNetwork): '<S97>/UDP Receive'
  sErr = GetErrorBuffer(&Parker_Velocity_Control_DW.UDPReceive_NetworkLib[0U]);
  LibTerminate(&Parker_Velocity_Control_DW.UDPReceive_NetworkLib[0U]);
  if (*sErr != 0) {
    rtmSetErrorStatus(Parker_Velocity_Control_M, sErr);
    rtmSetStopRequested(Parker_Velocity_Control_M, 1);
  }

  LibDestroy(&Parker_Velocity_Control_DW.UDPReceive_NetworkLib[0U], 0);
  DestroyUDPInterface(&Parker_Velocity_Control_DW.UDPReceive_NetworkLib[0U]);

  // End of Terminate for S-Function (sdspFromNetwork): '<S97>/UDP Receive'
  // End of Terminate for SubSystem: '<S6>/Use Hardware to Obtain States'

  // Terminate for IfAction SubSystem: '<S8>/Change Behavior'
  // Terminate for MATLABSystem: '<S128>/Read Joint Positions using  Dynamixel Encoders' 
  Parker_Veloci_matlabCodegenHa_h(&Parker_Velocity_Control_DW.obj_c);

  // End of Terminate for SubSystem: '<S8>/Change Behavior'

  // Terminate for S-Function (sdspToNetwork): '<S18>/UDP Send'
  sErr = GetErrorBuffer(&Parker_Velocity_Control_DW.UDPSend_NetworkLib[0U]);
  LibTerminate(&Parker_Velocity_Control_DW.UDPSend_NetworkLib[0U]);
  if (*sErr != 0) {
    rtmSetErrorStatus(Parker_Velocity_Control_M, sErr);
    rtmSetStopRequested(Parker_Velocity_Control_M, 1);
  }

  LibDestroy(&Parker_Velocity_Control_DW.UDPSend_NetworkLib[0U], 1);
  DestroyUDPInterface(&Parker_Velocity_Control_DW.UDPSend_NetworkLib[0U]);

  // End of Terminate for S-Function (sdspToNetwork): '<S18>/UDP Send'

  // Terminate for MATLABSystem: '<Root>/Initialization Block for Speed Control' 
  matlabCodegenHandle_matlabCodeg(&Parker_Velocity_Control_DW.obj);

  // Terminate for MATLABSystem: '<S17>/WhoAmI'
  Parker_Veloci_matlabCodegenHa_m(&Parker_Velocity_Control_DW.obj_b);

  // Terminate for Atomic SubSystem: '<S4>/Send Commands to PWM Blocks'
  // Terminate for MATLABSystem: '<S33>/RED T1 - BLACK T8'
  matlabCodegenHandle_matlabC_iri(&Parker_Velocity_Control_DW.obj_m);

  // Terminate for MATLABSystem: '<S33>/RED T2 - BLACK T3'
  matlabCodegenHandle_matlabC_iri(&Parker_Velocity_Control_DW.obj_ma);

  // Terminate for MATLABSystem: '<S33>/RED T3'
  matlabCodegenHandle_matlabC_iri(&Parker_Velocity_Control_DW.obj_h);

  // Terminate for MATLABSystem: '<S33>/RED T4 - BLACK T5'
  matlabCodegenHandle_matlabC_iri(&Parker_Velocity_Control_DW.obj_k);

  // Terminate for MATLABSystem: '<S33>/RED T5 - BLACK T4'
  matlabCodegenHandle_matlabC_iri(&Parker_Velocity_Control_DW.obj_hw);

  // Terminate for MATLABSystem: '<S33>/RED T6 - BLACK T7'
  matlabCodegenHandle_matlabC_iri(&Parker_Velocity_Control_DW.obj_hf);

  // Terminate for MATLABSystem: '<S33>/RED T7 - BLACK T6'
  matlabCodegenHandle_matlabC_iri(&Parker_Velocity_Control_DW.obj_ce);

  // Terminate for MATLABSystem: '<S33>/RED T8 - BLACK T1'
  matlabCodegenHandle_matlabC_iri(&Parker_Velocity_Control_DW.obj_fo);

  // Terminate for MATLABSystem: '<S33>/BLACK T2'
  matlabCodegenHandle_matlabC_iri(&Parker_Velocity_Control_DW.obj_ci);

  // End of Terminate for SubSystem: '<S4>/Send Commands to PWM Blocks'

  // Terminate for IfAction SubSystem: '<S5>/Change RED Behavior'
  // Terminate for Atomic SubSystem: '<S81>/Obtain RW Status'
  // Terminate for MATLABSystem: '<S92>/Digital Read'
  matlabCodegenHandl_iriacmixebgr(&Parker_Velocity_Control_DW.obj_f);

  // End of Terminate for SubSystem: '<S81>/Obtain RW Status'

  // Terminate for Atomic SubSystem: '<S81>/Send Direction to Motor Controller'
  // Terminate for MATLABSystem: '<S93>/Digital Write'
  matlabCodegenHand_iriacmixebgrh(&Parker_Velocity_Control_DW.obj_bm);

  // End of Terminate for SubSystem: '<S81>/Send Direction to Motor Controller'

  // Terminate for Atomic SubSystem: '<S81>/Send PWM to Motor Controller'
  // Terminate for MATLABSystem: '<S90>/PWM1'
  matlabCodegenHandle_matlabC_iri(&Parker_Velocity_Control_DW.obj_d);

  // End of Terminate for SubSystem: '<S81>/Send PWM to Motor Controller'

  // Terminate for Atomic SubSystem: '<S81>/Turn on Motor'
  // Terminate for MATLABSystem: '<S94>/Digital Write'
  matlabCodegenHa_iriacmixebgrhtp(&Parker_Velocity_Control_DW.obj_cp);

  // End of Terminate for SubSystem: '<S81>/Turn on Motor'
  // End of Terminate for SubSystem: '<S5>/Change RED Behavior'
}
