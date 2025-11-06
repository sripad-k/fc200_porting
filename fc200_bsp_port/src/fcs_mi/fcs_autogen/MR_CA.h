/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: MR_CA.h
 *
 * Code generated for Simulink model 'MR_CA'.
 *
 * Model version                  : 2.72
 * Simulink Coder version         : 24.2 (R2024b) 21-Jun-2024
 * Git Hash                       : 109356e0
 * C/C++ source code generated on : Mon Sep 15 11:51:44 2025
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: ARM Compatible->ARM Cortex-M
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#ifndef MR_CA_h_
#define MR_CA_h_
#ifndef MR_CA_COMMON_INCLUDES_
#define MR_CA_COMMON_INCLUDES_
#include "rtwtypes.h"
#endif                                 /* MR_CA_COMMON_INCLUDES_ */

#include "MR_CA_types.h"
#include <string.h>

extern void MR_CA_Init(void);
extern void MR_CA(const real_T *rtu_busControllerAltCtrl_forceDes, const real_T
                  rtu_busControllerAttCtrl_momentDes[3], const vom_t
                  *rtu_vom_status, const boolean_T *rtu_sFlags_rampup_phase,
                  const real_T *rtu_Sensor_aspd_cas, const lifter_state_t
                  *rtu_lifter_state, const lifter_state_t
                  *rtu_FW_LifterMode_eFWLifter_Mode, busControllerCA
                  *rty_controllerCA);

/* Model reference registration function */
extern void MR_CA_initialize(void);

/*-
 * These blocks were eliminated from the model due to optimizations:
 *
 * Block '<Root>/Reshape' : Reshape block reduction
 * Block '<S5>/Reshape' : Reshape block reduction
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
 * '<Root>' : 'MR_CA'
 * '<S1>'   : 'MR_CA/Subsystem'
 * '<S2>'   : 'MR_CA/set_idle_and_ready_pwm1'
 * '<S3>'   : 'MR_CA/Subsystem/Variant Subsystem'
 * '<S4>'   : 'MR_CA/Subsystem/outputFilter'
 * '<S5>'   : 'MR_CA/Subsystem/Variant Subsystem/fixed_ca'
 * '<S6>'   : 'MR_CA/Subsystem/Variant Subsystem/fixed_ca/MATLAB Function'
 * '<S7>'   : 'MR_CA/Subsystem/outputFilter/noFilter'
 * '<S8>'   : 'MR_CA/set_idle_and_ready_pwm1/Compare To Constant'
 * '<S9>'   : 'MR_CA/set_idle_and_ready_pwm1/Compare To Constant1'
 * '<S10>'  : 'MR_CA/set_idle_and_ready_pwm1/Compare To Constant4'
 * '<S11>'  : 'MR_CA/set_idle_and_ready_pwm1/Subsystem'
 * '<S12>'  : 'MR_CA/set_idle_and_ready_pwm1/Subsystem1'
 * '<S13>'  : 'MR_CA/set_idle_and_ready_pwm1/fill_array'
 * '<S14>'  : 'MR_CA/set_idle_and_ready_pwm1/Subsystem/Compare To Constant1'
 * '<S15>'  : 'MR_CA/set_idle_and_ready_pwm1/Subsystem/Compare To Constant3'
 * '<S16>'  : 'MR_CA/set_idle_and_ready_pwm1/Subsystem/Compare To Constant4'
 * '<S17>'  : 'MR_CA/set_idle_and_ready_pwm1/Subsystem/Compare To Constant5'
 * '<S18>'  : 'MR_CA/set_idle_and_ready_pwm1/Subsystem/Compare To Constant6'
 * '<S19>'  : 'MR_CA/set_idle_and_ready_pwm1/Subsystem/Compare To Constant7'
 * '<S20>'  : 'MR_CA/set_idle_and_ready_pwm1/Subsystem1/Compare To Constant'
 * '<S21>'  : 'MR_CA/set_idle_and_ready_pwm1/Subsystem1/Compare To Constant1'
 * '<S22>'  : 'MR_CA/set_idle_and_ready_pwm1/Subsystem1/Compare To Constant2'
 * '<S23>'  : 'MR_CA/set_idle_and_ready_pwm1/Subsystem1/Compare To Constant3'
 * '<S24>'  : 'MR_CA/set_idle_and_ready_pwm1/Subsystem1/Compare To Constant4'
 * '<S25>'  : 'MR_CA/set_idle_and_ready_pwm1/Subsystem1/Compare To Constant5'
 * '<S26>'  : 'MR_CA/set_idle_and_ready_pwm1/Subsystem1/Compare To Constant6'
 * '<S27>'  : 'MR_CA/set_idle_and_ready_pwm1/Subsystem1/Compare To Constant7'
 * '<S28>'  : 'MR_CA/set_idle_and_ready_pwm1/fill_array/SS'
 */

/*-
 * Requirements for '<Root>': MR_CA

 */
#endif                                 /* MR_CA_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
