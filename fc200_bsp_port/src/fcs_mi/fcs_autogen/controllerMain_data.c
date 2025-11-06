/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: controllerMain_data.c
 *
 * Code generated for Simulink model 'controllerMain'.
 *
 * Model version                  : 3.599
 * Simulink Coder version         : 24.2 (R2024b) 21-Jun-2024
 * Git Hash                       : 109356e0
 * C/C++ source code generated on : Mon Sep 15 11:53:18 2025
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: ARM Compatible->ARM Cortex-M
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#include "controllerMain.h"

/* Invariant block signals (default storage) */
const controllerMain_fauf41phob_T controllerMain_ConstB = {
  -1.0,                                /* '<S15>/Gain' */
  0.015000000000000013,                /* '<S23>/Sum1' */
  9.80665,                             /* '<S28>/Abs' */
  1.0,                                 /* '<S28>/Switch1' */
  0.001,                               /* '<S28>/Product' */
  9.80665                              /* '<S28>/Switch3' */
};

/* Constant parameters (default storage) */
const controllerMain_dfsmisgjtw_T controllerMain_ConstP = {
  /* Expression: [0, 0.7]
   * Referenced by: '<S1>/1-D Lookup Table'
   */
  { 0.0, 0.7 },

  /* Expression: [10, 17]
   * Referenced by: '<S1>/1-D Lookup Table'
   */
  { 10.0, 17.0 }
};

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
