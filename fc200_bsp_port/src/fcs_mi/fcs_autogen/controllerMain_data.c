/*
 * File: controllerMain_data.c
 *
 * Code generated for Simulink model 'controllerMain'.
 *
 * Model version                  : 3.618
 * Simulink Coder version         : 24.2 (R2024b) 21-Jun-2024
 * Git Hash                       : 4a0df9c8
 * C/C++ source code generated on : Thu Nov 27 13:07:43 2025
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: ARM Compatible->ARM Cortex-M
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#include "controllerMain.h"

/* Invariant block signals (default storage) */
const controllerMain_fauf41phob_T controllerMain_ConstB = {
  -1.0,                                /* '<S16>/Gain' */
  0.015000000000000013,                /* '<S24>/Sum1' */
  9.80665,                             /* '<S29>/Abs' */
  1.0,                                 /* '<S29>/Switch1' */
  0.001,                               /* '<S29>/Product' */
  9.80665                              /* '<S29>/Switch3' */
};

/* Constant parameters (default storage) */
const controllerMain_dfsmisgjtw_T controllerMain_ConstP = {
  /* Expression: transition_lambda_data
   * Referenced by: '<S1>/1-D Lookup Table'
   */
  { 0.0, 1.0 },

  /* Expression: transition_lambda_bp
   * Referenced by: '<S1>/1-D Lookup Table'
   */
  { 10.0, 22.0 }
};

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
