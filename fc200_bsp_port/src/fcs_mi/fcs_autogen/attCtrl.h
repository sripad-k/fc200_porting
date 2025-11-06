/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: attCtrl.h
 *
 * Code generated for Simulink model 'attCtrl'.
 *
 * Model version                  : 2.67
 * Simulink Coder version         : 24.2 (R2024b) 21-Jun-2024
 * Git Hash                       : 109356e0
 * C/C++ source code generated on : Mon Sep 15 11:52:03 2025
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: ARM Compatible->ARM Cortex-M
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#ifndef attCtrl_h_
#define attCtrl_h_
#ifndef attCtrl_COMMON_INCLUDES_
#define attCtrl_COMMON_INCLUDES_
#include "rtwtypes.h"
#endif                                 /* attCtrl_COMMON_INCLUDES_ */

#include "attCtrl_types.h"
#include <string.h>

extern void attCtrl_Init(void);
extern void attCtrl(const real_T *rtu_busControllerIF_attAlt_rollCmd, const
                    real_T *rtu_busControllerIF_attAlt_pitchCmd, const real_T
                    *rtu_busControllerIF_attAlt_yawCmd, const real_T
                    *rtu_busControllerIF_attAlt_yawRateCmd, const boolean_T
                    *rtu_busControllerIF_attAlt_yawHold, const real_T
                    *rtu_busControllerIF_attAlt_yawFF, const boolean_T
                    *rtu_busControllerIF_attAlt_init_reset, const std_sensor_t
                    *rtu_Sensor, const real_T *rtu_busControllerCA_c_erp1, const
                    real_T *rtu_busControllerCA_c_erp3, const real_T
                    *rtu_MR_IntCF_roll_CF, const real_T *rtu_MR_IntCF_pitch_CF,
                    const real_T *rtu_MR_IntCF_lambda, busControllerAttCtrl
                    *rty_controllerAttCtrl, real_T *rty_MR_Int_roll_int, real_T *
                    rty_MR_Int_pitch_int, real_T *rty_MR_Int_roll_sat, real_T
                    *rty_MR_Int_pitch_sat);

/* Model reference registration function */
extern void attCtrl_initialize(void);

/*-
 * These blocks were eliminated from the model due to optimizations:
 *
 * Block '<S19>/Data Type Duplicate' : Unused code path elimination
 * Block '<S19>/Data Type Propagation' : Unused code path elimination
 */

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
 * '<Root>' : 'attCtrl'
 * '<S1>'   : 'attCtrl/AttCtrl_Variant'
 * '<S2>'   : 'attCtrl/EC_yawRate'
 * '<S3>'   : 'attCtrl/kinematicInversion'
 * '<S4>'   : 'attCtrl/omega2eulerdot'
 * '<S5>'   : 'attCtrl/yaw_ctrl'
 * '<S6>'   : 'attCtrl/AttCtrl_Variant/P-PID'
 * '<S7>'   : 'attCtrl/AttCtrl_Variant/P-PID/AW_clamping'
 * '<S8>'   : 'attCtrl/AttCtrl_Variant/P-PID/Subsystem'
 * '<S9>'   : 'attCtrl/AttCtrl_Variant/P-PID/derivativeFilter'
 * '<S10>'  : 'attCtrl/AttCtrl_Variant/P-PID/inputFilter'
 * '<S11>'  : 'attCtrl/AttCtrl_Variant/P-PID/AW_clamping/Compare To Constant'
 * '<S12>'  : 'attCtrl/AttCtrl_Variant/P-PID/AW_clamping/Compare To Zero'
 * '<S13>'  : 'attCtrl/AttCtrl_Variant/P-PID/Subsystem/Compare To Constant'
 * '<S14>'  : 'attCtrl/AttCtrl_Variant/P-PID/Subsystem/Compare To Constant1'
 * '<S15>'  : 'attCtrl/EC_yawRate/AW_clamping'
 * '<S16>'  : 'attCtrl/EC_yawRate/AW_clamping/Compare To Constant'
 * '<S17>'  : 'attCtrl/EC_yawRate/AW_clamping/Compare To Zero'
 * '<S18>'  : 'attCtrl/omega2eulerdot/MATLAB Function'
 * '<S19>'  : 'attCtrl/yaw_ctrl/Saturation Dynamic3'
 * '<S20>'  : 'attCtrl/yaw_ctrl/wrapToPi'
 */

/*-
 * Requirements for '<Root>': attCtrl

 */
#endif                                 /* attCtrl_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
