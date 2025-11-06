/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: attCtrl_data.c
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

#include "attCtrl_private.h"

/* Constant parameters (default storage) */
const attCtrl_mlvuba4hqv_T attCtrl_ConstP = {
  /* Expression: [0,1]
   * Referenced by: '<Root>/1-D Lookup Table'
   */
  { 0.0, 1.0 },

  /* Expression: [7, 17]
   * Referenced by: '<Root>/1-D Lookup Table'
   */
  { 7.0, 17.0 },

  /* Expression: inertial_J
   * Referenced by: '<Root>/Multiply'
   */
  { 43.46, 0.0, 0.0, 0.0, 38.84, 0.0, 0.0, 0.0, 74.36 }
};

/* Invariant block signals (default storage) */
const attCtrl_TConstB attCtrl_ConstB = {
  0.0,                                 /* '<S6>/integralResetValue' */
  0.0,                                 /* '<S2>/integralResetValue' */
  -0.52359877559829882,                /* '<S5>/Gain1' */
  6.2831853071795862                   /* '<S20>/Sum1' */
};

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
