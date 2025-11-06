/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: transitionHandler.h
 *
 * Code generated for Simulink model 'transitionHandler'.
 *
 * Model version                  : 2.22
 * Simulink Coder version         : 24.2 (R2024b) 21-Jun-2024
 * Git Hash                       : 109356e0
 * C/C++ source code generated on : Mon Sep 15 11:52:52 2025
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: ARM Compatible->ARM Cortex-M
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#ifndef transitionHandler_h_
#define transitionHandler_h_
#ifndef transitionHandler_COMMON_INCLUDES_
#define transitionHandler_COMMON_INCLUDES_
#include "rtwtypes.h"
#endif                                 /* transitionHandler_COMMON_INCLUDES_ */

#include "transitionHandler_types.h"

extern void transitionHandler(const busControllerCA *rtu_CA_MR, const
  busControllerCA *rtu_CA_FW, std_ctrl_t *rty_stdCtrl_IF, busControllerCA
  *rty_controllerCA_mix);

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
 * '<Root>' : 'transitionHandler'
 */

/*-
 * Requirements for '<Root>': transitionHandler

 */
#endif                                 /* transitionHandler_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
