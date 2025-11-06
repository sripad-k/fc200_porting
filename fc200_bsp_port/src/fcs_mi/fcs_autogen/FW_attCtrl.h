/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: FW_attCtrl.h
 *
 * Code generated for Simulink model 'FW_attCtrl'.
 *
 * Model version                  : 2.68
 * Simulink Coder version         : 24.2 (R2024b) 21-Jun-2024
 * Git Hash                       : 109356e0
 * C/C++ source code generated on : Mon Sep 15 11:51:07 2025
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: ARM Compatible->ARM Cortex-M
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#ifndef FW_attCtrl_h_
#define FW_attCtrl_h_
#ifndef FW_attCtrl_COMMON_INCLUDES_
#define FW_attCtrl_COMMON_INCLUDES_
#include "rtwtypes.h"
#endif                                 /* FW_attCtrl_COMMON_INCLUDES_ */

#include "FW_attCtrl_types.h"
#include <string.h>

extern void FW_attCtrl_Init(void);
extern void FW_attCtrl(const real_T *rtu_FW_Controller_att_pitch_cmd, const
  real_T *rtu_FW_Controller_att_roll_cmd, const boolean_T
  *rtu_FW_Controller_att_roll_reset, const boolean_T
  *rtu_FW_Controller_att_pitch_reset, const std_sensor_t *rtu_Sensor, const
  real_T *rtu_controllerCA_FW_c_erp1, const real_T *rtu_controllerCA_FW_c_erp2,
  const real_T *rtu_FW_IntCF_roll_CF, const real_T *rtu_FW_IntCF_pitch_CF,
  real_T *rty_ele_L, real_T *rty_ail_LO, real_T *rty_rud_L, real_T
  *rty_FW_IntData_roll_int, real_T *rty_FW_IntData_pitch_int, real_T
  *rty_FW_IntData_roll_sat, real_T *rty_FW_IntData_pitch_sat, real_T
  *rty_FwAttCmd_pitchCmd, real_T *rty_FwAttCmd_rollCmd, real_T
  *rty_FwAttCmd_raw_pitchCmd, real_T *rty_FwAttCmd_raw_rollCmd);

/* Model reference registration function */
extern void FW_attCtrl_initialize(void);

/*-
 * The generated code includes comments that allow you to trace directly
 * back to the appropriate location in the model.  The basic format
 * is <system>/block_name, where system is the system number (uniquely
 * assigned by Simulink) and block_name is the name of the block.
 *
 * Use the MATLAB hilite_system command to trace the generated code back
 * to the model.  For example,
 *
 * hilite_system('<S3>')    - opens system 3
 * hilite_system('<S3>/Kp') - opens and selects block Kp which resides in S3
 *
 * Here is the system hierarchy for this model
 *
 * '<Root>' : 'FW_attCtrl'
 * '<S1>'   : 'FW_attCtrl/inputFilter'
 * '<S2>'   : 'FW_attCtrl/inputFilter1'
 * '<S3>'   : 'FW_attCtrl/long'
 * '<S4>'   : 'FW_attCtrl/roll'
 * '<S5>'   : 'FW_attCtrl/yaw'
 * '<S6>'   : 'FW_attCtrl/long/AW_clamping'
 * '<S7>'   : 'FW_attCtrl/long/Subsystem'
 * '<S8>'   : 'FW_attCtrl/long/euler2dcm'
 * '<S9>'   : 'FW_attCtrl/long/AW_clamping/Compare To Constant'
 * '<S10>'  : 'FW_attCtrl/long/AW_clamping/Compare To Zero'
 * '<S11>'  : 'FW_attCtrl/long/Subsystem/Compare To Constant1'
 * '<S12>'  : 'FW_attCtrl/long/euler2dcm/MATLAB Function'
 * '<S13>'  : 'FW_attCtrl/roll/AW_clamping'
 * '<S14>'  : 'FW_attCtrl/roll/Subsystem'
 * '<S15>'  : 'FW_attCtrl/roll/AW_clamping/Compare To Constant'
 * '<S16>'  : 'FW_attCtrl/roll/AW_clamping/Compare To Zero'
 * '<S17>'  : 'FW_attCtrl/roll/Subsystem/Compare To Constant1'
 */

/*-
 * Requirements for '<Root>': FW_attCtrl

 */
#endif                                 /* FW_attCtrl_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
