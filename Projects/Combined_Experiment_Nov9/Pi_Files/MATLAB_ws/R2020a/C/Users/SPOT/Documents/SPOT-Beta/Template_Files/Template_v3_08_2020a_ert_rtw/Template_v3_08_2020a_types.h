//
//  Template_v3_08_2020a_types.h
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


#ifndef RTW_HEADER_Template_v3_08_2020a_types_h_
#define RTW_HEADER_Template_v3_08_2020a_types_h_
#include "rtwtypes.h"
#include "builtin_typeid_types.h"
#include "multiword_types.h"

// Model Code Variants
#ifndef struct_tag_yioZfVzJFClp3vmW2NrXQC
#define struct_tag_yioZfVzJFClp3vmW2NrXQC

struct tag_yioZfVzJFClp3vmW2NrXQC
{
  int32_T __dummy;
};

#endif                                 //struct_tag_yioZfVzJFClp3vmW2NrXQC

#ifndef typedef_c_fusion_internal_frames_NED__T
#define typedef_c_fusion_internal_frames_NED__T

typedef struct tag_yioZfVzJFClp3vmW2NrXQC c_fusion_internal_frames_NED__T;

#endif                                 //typedef_c_fusion_internal_frames_NED__T

#ifndef struct_tag_owjKvqUcBKvgeoFYRvk4MD
#define struct_tag_owjKvqUcBKvgeoFYRvk4MD

struct tag_owjKvqUcBKvgeoFYRvk4MD
{
  real_T a;
  real_T b;
  real_T c;
  real_T d;
};

#endif                                 //struct_tag_owjKvqUcBKvgeoFYRvk4MD

#ifndef typedef_g_matlabshared_rotations_inte_T
#define typedef_g_matlabshared_rotations_inte_T

typedef struct tag_owjKvqUcBKvgeoFYRvk4MD g_matlabshared_rotations_inte_T;

#endif                                 //typedef_g_matlabshared_rotations_inte_T

#ifndef struct_tag_PMfBDzoakfdM9QAdfx2o6D
#define struct_tag_PMfBDzoakfdM9QAdfx2o6D

struct tag_PMfBDzoakfdM9QAdfx2o6D
{
  uint32_T f1[8];
};

#endif                                 //struct_tag_PMfBDzoakfdM9QAdfx2o6D

#ifndef typedef_cell_wrap_Template_v3_08_2020_T
#define typedef_cell_wrap_Template_v3_08_2020_T

typedef struct tag_PMfBDzoakfdM9QAdfx2o6D cell_wrap_Template_v3_08_2020_T;

#endif                                 //typedef_cell_wrap_Template_v3_08_2020_T

#ifndef struct_tag_MnuwZGhgu8kjqV344GyhbE
#define struct_tag_MnuwZGhgu8kjqV344GyhbE

struct tag_MnuwZGhgu8kjqV344GyhbE
{
  int32_T isInitialized;
  boolean_T TunablePropsChanged;
  cell_wrap_Template_v3_08_2020_T inputVarSize[3];
  real_T AccelerometerNoise;
  real_T GyroscopeNoise;
  real_T GyroscopeDriftNoise;
  real_T LinearAccelerationNoise;
  real_T LinearAccelerationDecayFactor;
  real_T pQw[144];
  real_T pQv[36];
  g_matlabshared_rotations_inte_T pOrientPost;
  g_matlabshared_rotations_inte_T pOrientPrior;
  boolean_T pFirstTime;
  c_fusion_internal_frames_NED__T *pRefSys;
  real_T pSensorPeriod;
  real_T pKalmanPeriod;
  real_T pGyroOffset[3];
  real_T pLinAccelPrior[3];
  real_T pLinAccelPost[3];
  real_T pInputPrototype[3];
  real_T MagnetometerNoise;
  real_T MagneticDisturbanceNoise;
  real_T MagneticDisturbanceDecayFactor;
  real_T ExpectedMagneticFieldStrength;
  real_T pMagVec[3];
};

#endif                                 //struct_tag_MnuwZGhgu8kjqV344GyhbE

#ifndef typedef_fusion_simulink_ahrsfilter_Te_T
#define typedef_fusion_simulink_ahrsfilter_Te_T

typedef struct tag_MnuwZGhgu8kjqV344GyhbE fusion_simulink_ahrsfilter_Te_T;

#endif                                 //typedef_fusion_simulink_ahrsfilter_Te_T

#ifndef struct_tag_aH9AaDy6ZLSd2jSOyjaJQC
#define struct_tag_aH9AaDy6ZLSd2jSOyjaJQC

struct tag_aH9AaDy6ZLSd2jSOyjaJQC
{
  boolean_T matlabCodegenIsDeleted;
  int32_T isInitialized;
  boolean_T isSetupComplete;
};

#endif                                 //struct_tag_aH9AaDy6ZLSd2jSOyjaJQC

#ifndef typedef_codertarget_linux_blocks_Digi_T
#define typedef_codertarget_linux_blocks_Digi_T

typedef struct tag_aH9AaDy6ZLSd2jSOyjaJQC codertarget_linux_blocks_Digi_T;

#endif                                 //typedef_codertarget_linux_blocks_Digi_T

#ifndef struct_tag_LZxdu5j29nU7JY80L8iTaC
#define struct_tag_LZxdu5j29nU7JY80L8iTaC

struct tag_LZxdu5j29nU7JY80L8iTaC
{
  boolean_T matlabCodegenIsDeleted;
  boolean_T isSetupComplete;
};

#endif                                 //struct_tag_LZxdu5j29nU7JY80L8iTaC

#ifndef typedef_MoveArm_Torque_Template_v3_08_T
#define typedef_MoveArm_Torque_Template_v3_08_T

typedef struct tag_LZxdu5j29nU7JY80L8iTaC MoveArm_Torque_Template_v3_08_T;

#endif                                 //typedef_MoveArm_Torque_Template_v3_08_T

#ifndef struct_tag_mJhNKAE3yyZ8cCBuxCC07F
#define struct_tag_mJhNKAE3yyZ8cCBuxCC07F

struct tag_mJhNKAE3yyZ8cCBuxCC07F
{
  boolean_T matlabCodegenIsDeleted;
  int32_T isInitialized;
  boolean_T isSetupComplete;
  uint32_T PinNumber;
};

#endif                                 //struct_tag_mJhNKAE3yyZ8cCBuxCC07F

#ifndef typedef_raspi_internal_PWMBlock_Templ_T
#define typedef_raspi_internal_PWMBlock_Templ_T

typedef struct tag_mJhNKAE3yyZ8cCBuxCC07F raspi_internal_PWMBlock_Templ_T;

#endif                                 //typedef_raspi_internal_PWMBlock_Templ_T

// Custom Type definition for MATLABSystem: '<S6>/LSM9DS1 IMU Sensor'
#include "MW_SVD.h"
#include "MW_I2C.h"
#ifndef struct_tag_R1xdlxXgEuZ2YKjrgC1tPF
#define struct_tag_R1xdlxXgEuZ2YKjrgC1tPF

struct tag_R1xdlxXgEuZ2YKjrgC1tPF
{
  int32_T __dummy;
};

#endif                                 //struct_tag_R1xdlxXgEuZ2YKjrgC1tPF

#ifndef typedef_e_codertarget_raspi_internal__T
#define typedef_e_codertarget_raspi_internal__T

typedef struct tag_R1xdlxXgEuZ2YKjrgC1tPF e_codertarget_raspi_internal__T;

#endif                                 //typedef_e_codertarget_raspi_internal__T

#ifndef struct_tag_wSflavgv0iM67EP96OKIZ
#define struct_tag_wSflavgv0iM67EP96OKIZ

struct tag_wSflavgv0iM67EP96OKIZ
{
  boolean_T matlabCodegenIsDeleted;
  int32_T isInitialized;
  e_codertarget_raspi_internal__T Hw;
  uint32_T BusSpeed;
  MW_Handle_Type MW_I2C_HANDLE;
};

#endif                                 //struct_tag_wSflavgv0iM67EP96OKIZ

#ifndef typedef_i_codertarget_raspi_internal__T
#define typedef_i_codertarget_raspi_internal__T

typedef struct tag_wSflavgv0iM67EP96OKIZ i_codertarget_raspi_internal__T;

#endif                                 //typedef_i_codertarget_raspi_internal__T

#ifndef struct_tag_wPlwB1N2byXVSJAk9BGO9E
#define struct_tag_wPlwB1N2byXVSJAk9BGO9E

struct tag_wPlwB1N2byXVSJAk9BGO9E
{
  boolean_T matlabCodegenIsDeleted;
  int32_T isInitialized;
  e_codertarget_raspi_internal__T Hw;
  uint32_T BusSpeed;
  MW_Handle_Type MW_I2C_HANDLE;
};

#endif                                 //struct_tag_wPlwB1N2byXVSJAk9BGO9E

#ifndef typedef_j_codertarget_raspi_internal__T
#define typedef_j_codertarget_raspi_internal__T

typedef struct tag_wPlwB1N2byXVSJAk9BGO9E j_codertarget_raspi_internal__T;

#endif                                 //typedef_j_codertarget_raspi_internal__T

#ifndef struct_tag_x0PTJOOFtVdlhp8iMPqZXD
#define struct_tag_x0PTJOOFtVdlhp8iMPqZXD

struct tag_x0PTJOOFtVdlhp8iMPqZXD
{
  boolean_T matlabCodegenIsDeleted;
  int32_T isInitialized;
  boolean_T isSetupComplete;
  real_T SampleTime;
};

#endif                                 //struct_tag_x0PTJOOFtVdlhp8iMPqZXD

#ifndef typedef_ReadArm_Position_Rates_Templa_T
#define typedef_ReadArm_Position_Rates_Templa_T

typedef struct tag_x0PTJOOFtVdlhp8iMPqZXD ReadArm_Position_Rates_Templa_T;

#endif                                 //typedef_ReadArm_Position_Rates_Templa_T

#ifndef struct_tag_2kBT7x5hWRpJpWP4VQpqED
#define struct_tag_2kBT7x5hWRpJpWP4VQpqED

struct tag_2kBT7x5hWRpJpWP4VQpqED
{
  boolean_T matlabCodegenIsDeleted;
  int32_T isInitialized;
  boolean_T isSetupComplete;
};

#endif                                 //struct_tag_2kBT7x5hWRpJpWP4VQpqED

#ifndef typedef_InitializeForTorque_Template__T
#define typedef_InitializeForTorque_Template__T

typedef struct tag_2kBT7x5hWRpJpWP4VQpqED InitializeForTorque_Template__T;

#endif                                 //typedef_InitializeForTorque_Template__T

#ifndef struct_tag_vXY470eI9qtNjsBz4FTO0D
#define struct_tag_vXY470eI9qtNjsBz4FTO0D

struct tag_vXY470eI9qtNjsBz4FTO0D
{
  boolean_T matlabCodegenIsDeleted;
  int32_T isInitialized;
  boolean_T isSetupComplete;
  real_T SampleTime;
};

#endif                                 //struct_tag_vXY470eI9qtNjsBz4FTO0D

#ifndef typedef_IdentifyPlatform_Template_v3__T
#define typedef_IdentifyPlatform_Template_v3__T

typedef struct tag_vXY470eI9qtNjsBz4FTO0D IdentifyPlatform_Template_v3__T;

#endif                                 //typedef_IdentifyPlatform_Template_v3__T

#ifndef struct_tag_Mu1SNMC7dAyyNmAihLbeZF
#define struct_tag_Mu1SNMC7dAyyNmAihLbeZF

struct tag_Mu1SNMC7dAyyNmAihLbeZF
{
  boolean_T matlabCodegenIsDeleted;
  int32_T isInitialized;
  boolean_T isSetupComplete;
  real_T CalGyroA[9];
  real_T CalGyroB[3];
  real_T CalAccelA[9];
  real_T CalAccelB[3];
  real_T CalMagA[9];
  real_T CalMagB[3];
  i_codertarget_raspi_internal__T i2cobj_A_G;
  j_codertarget_raspi_internal__T i2cobj_MAG;
};

#endif                                 //struct_tag_Mu1SNMC7dAyyNmAihLbeZF

#ifndef typedef_raspi_internal_lsm9ds1Block_T_T
#define typedef_raspi_internal_lsm9ds1Block_T_T

typedef struct tag_Mu1SNMC7dAyyNmAihLbeZF raspi_internal_lsm9ds1Block_T_T;

#endif                                 //typedef_raspi_internal_lsm9ds1Block_T_T

// Parameters for system: '<S109>/AHRS2'
typedef struct P_AHRS2_Template_v3_08_2020a_T_ P_AHRS2_Template_v3_08_2020a_T;

// Parameters for system: '<S119>/Calculate Running Mean'
typedef struct P_CalculateRunningMean_Templa_T_ P_CalculateRunningMean_Templa_T;

// Parameters for system: '<S134>/Calculate Running Mean'
typedef struct P_CalculateRunningMean_Temp_k_T_ P_CalculateRunningMean_Temp_k_T;

// Parameters for system: '<S10>/Change BLUE Behavior'
typedef struct P_ChangeBLUEBehavior_Template_T_ P_ChangeBLUEBehavior_Template_T;

// Parameters for system: '<Root>/Phase #0:  Wait for Synchronization'
typedef struct P_Phase0WaitforSynchronizatio_T_ P_Phase0WaitforSynchronizatio_T;

// Parameters for system: '<S228>/RED+ARM'
typedef struct P_REDARM_Template_v3_08_2020a_T_ P_REDARM_Template_v3_08_2020a_T;

// Parameters for system: '<S280>/Sub-Phase #1'
typedef struct P_SubPhase1_Template_v3_08_20_T_ P_SubPhase1_Template_v3_08_20_T;

// Parameters for system: '<S281>/Sub-Phase #1'
typedef struct P_SubPhase1_Template_v3_08__k_T_ P_SubPhase1_Template_v3_08__k_T;

// Parameters for system: '<S437>/RED+ARM'
typedef struct P_REDARM_Template_v3_08_202_a_T_ P_REDARM_Template_v3_08_202_a_T;

// Parameters for system: '<S14>/Change BLUE Behavior'
typedef struct P_ChangeBLUEBehavior_Templa_c_T_ P_ChangeBLUEBehavior_Templa_c_T;

// Parameters (default storage)
typedef struct P_Template_v3_08_2020a_T_ P_Template_v3_08_2020a_T;

// Forward declaration for rtModel
typedef struct tag_RTM_Template_v3_08_2020a_T RT_MODEL_Template_v3_08_2020a_T;

#endif                              // RTW_HEADER_Template_v3_08_2020a_types_h_
